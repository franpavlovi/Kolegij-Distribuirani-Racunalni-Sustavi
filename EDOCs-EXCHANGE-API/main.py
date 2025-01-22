from fastapi import FastAPI, File, UploadFile, Depends, HTTPException, BackgroundTasks
from fastapi.security import OAuth2PasswordBearer, OAuth2PasswordRequestForm
from jose import JWTError, jwt
from passlib.context import CryptContext
from smtplib import SMTP
from email.message import EmailMessage
import shutil
import os

app = FastAPI()

SECRET_KEY = "M2tDk9B8s7YxQvP3L6zR1aTgFpNlWnOa"
ALGORITHM = "HS256"
UPLOAD_FOLDER = "./uploaded_documents/"
ALLOWED_EXTENSIONS = {".pdf", ".docx"}

oauth2_scheme = OAuth2PasswordBearer(tokenUrl="token")
password_context = CryptContext(schemes=["bcrypt"], deprecated="auto")

users_db = {
    "user": {"username": "user", "password": password_context.hash("password"), "role": "USER"},
    "admin": {"username": "admin", "password": password_context.hash("admin"), "role": "ADMIN"},
}

def create_jwt_token(data: dict):
    return jwt.encode(data, SECRET_KEY, algorithm=ALGORITHM)

def verify_token(token: str):
    try:
        payload = jwt.decode(token, SECRET_KEY, algorithms=[ALGORITHM])
        return payload
    except JWTError:
        raise HTTPException(status_code=401, detail="Invalid token")

def check_user_role(token: str, required_role: str):
    user = verify_token(token)
    if user["role"] != required_role:
        raise HTTPException(status_code=403, detail="Permission denied")

@app.post("/token")
async def login(form_data: OAuth2PasswordRequestForm = Depends()):
    user = users_db.get(form_data.username)
    if not user or not password_context.verify(form_data.password, user["password"]):
        raise HTTPException(status_code=401, detail="Invalid credentials")
    token = create_jwt_token({"sub": user["username"], "role": user["role"]})
    return {"access_token": token, "token_type": "bearer"}

@app.post("/documents")
async def upload_document(file: UploadFile = File(...), token: str = Depends(oauth2_scheme)):
    check_user_role(token, "ADMIN")
    ext = os.path.splitext(file.filename)[1]
    if ext.lower() not in ALLOWED_EXTENSIONS:
        raise HTTPException(status_code=400, detail="File type not allowed")
    os.makedirs(UPLOAD_FOLDER, exist_ok=True)
    file_path = os.path.join(UPLOAD_FOLDER, file.filename)
    with open(file_path, "wb") as buffer:
        shutil.copyfileobj(file.file, buffer)
    return {"filename": file.filename}

@app.get("/documents/{filename}")
async def get_document(filename: str, token: str = Depends(oauth2_scheme)):
    verify_token(token)
    file_path = os.path.join(UPLOAD_FOLDER, filename)
    if not os.path.exists(file_path):
        raise HTTPException(status_code=404, detail="File not found")
    return {"filename": filename}

SMTP_SERVER = "smtp.gmail.com"
SMTP_PORT = 587
SMTP_USERNAME = "testiranjeemail94@gmail.com"
SMTP_PASSWORD = "zbdqqwfeokfabeif"

def send_email(email_to: str, subject: str, body: str, file_path: str):
    msg = EmailMessage()
    msg["Subject"] = subject
    msg["From"] = SMTP_USERNAME
    msg["To"] = email_to
    msg.set_content(body)
    with open(file_path, "rb") as f:
        msg.add_attachment(f.read(), maintype="application", subtype="octet-stream", filename=os.path.basename(file_path))
    with SMTP(SMTP_SERVER, SMTP_PORT) as smtp:
        smtp.starttls()
        smtp.login(SMTP_USERNAME, SMTP_PASSWORD)
        smtp.send_message(msg)

@app.post("/documents/{filename}/send")
async def send_document(filename: str, email: str, token: str = Depends(oauth2_scheme), background_tasks: BackgroundTasks = BackgroundTasks()):
    check_user_role(token, "ADMIN")
    file_path = os.path.join(UPLOAD_FOLDER, filename)
    if not os.path.exists(file_path):
        raise HTTPException(status_code=404, detail="File not found")
    background_tasks.add_task(send_email, email, "Your Document", "Please find attached document.", file_path)
    return {"message": f"Document {filename} is being sent to {email}"}

from flask import Flask, request, jsonify
from flask import send_file

from dotenv import load_dotenv
load_dotenv()
import subprocess
import requests
import os

app = Flask(__name__)

@app.route("/run", methods=["POST"])
def run_code():
    code = request.json["code"]

    with open("input.txt", "w", encoding="utf-8") as f:
        f.write(code)


    result = subprocess.run(["a.exe", "input.txt"],
                        capture_output=True, text=True,encoding="utf-8",errors="ignore")

    return jsonify({
        "output": result.stdout,
        "error": result.stderr
    })
api_key = os.getenv("GROQ_API_KEY")

@app.route("/chat", methods=["POST"])
def chat():
    try:
        user_msg = request.json["message"]

        response = requests.post(
            "https://api.groq.com/openai/v1/chat/completions",
            headers={
                "Authorization": "Bearer " + api_key,
                "Content-Type": "application/json"
            },
            json={
                "model": "openai/gpt-oss-20b",
                "messages": [
                    {"role": "user", "content": user_msg}
                ]
            }
        )

        data = response.json()

        if "choices" in data:
            reply = data["choices"][0]["message"]["content"]
        elif "error" in data:
            reply = "API Error: " + data["error"]["message"]
        else:
            reply = "Unknown error"

        return jsonify({"reply": reply})

    except Exception as e:
        print("ERROR:", e) 
        return jsonify({"reply": "Server error"}), 500

@app.route("/")
def home():
    return send_file("index.html")
    # return open("index.html", encoding="utf-8").read()



    

if __name__ == "__main__":
    port = int(os.environ.get("PORT", 5000))
    app.run(host="0.0.0.0", port=port)

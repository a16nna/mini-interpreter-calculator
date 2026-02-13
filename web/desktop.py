web/desktop.py
import threading
import webview
from server import app

def run_flask():
    app.run(port=5000, debug=False, use_reloader=False)

if __name__ == "__main__":
    flask_thread = threading.Thread(target=run_flask)
    flask_thread.daemon = True
    flask_thread.start()

    webview.create_window(
    "Mini Interpreter Calculator",
    "http://127.0.0.1:5000",
    width=400,
    height=600,
    resizable=True,
    transparent=False,
    frameless=False
)


    webview.start()

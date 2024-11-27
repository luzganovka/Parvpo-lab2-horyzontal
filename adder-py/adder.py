from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/', methods=['POST'])
def handle_post():
    
    data = request.data()
    print("Received data:", data)

    # Respond back with a success message
    return jsonify({"message": "Data received!", "data": data}), 200

if __name__ == '__main__':
    app.run(debug=False, port=8080)
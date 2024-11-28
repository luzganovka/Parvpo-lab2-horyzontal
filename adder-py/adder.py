from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/', methods=['POST'])
def handle_post():
    print("ADDER: Got post request")
    data = request.data
    print("ADDER: Received data:", data)

    # Respond back with a success message
    return jsonify({"message": "Data received!", "data": str(data)}), 200

if __name__ == '__main__':
    print("ADDER: Hello, world!")
    app.run(debug=False, host="0.0.0.0", port=8080)
    print("ADDER: Goodbye!")
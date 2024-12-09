from flask import Flask, request, jsonify

app = Flask(__name__)


def combine():
    global p_num
    global pieces

    print("ADDER: Combining...", flush=True)

    Nan = False
    for i in range(0, p_num):
        if pieces[i] == -1:
            Nan = True
        print("ADDER: combine check: ", pieces[i] == -1)

    if Nan:
        print("ADDER: Not all pieces recieved", flush=True)
        return 1
    else:
        for i in range(1, p_num):
            pieces[0] += pieces[i]
        print("ADDER: DATA COMBINED SECCESSFULLY!", flush=True)
        return 0

@app.route('/', methods=['POST'])
def handle_post():
    global p_num
    global pieces

    print("ADDER: Got post request", flush=True)
    data = request.data.decode("utf-8")
    print("ADDER: Received data:", data[:50], flush=True)

    if (p_num == -1):
        p_num = int(request.headers['p-num'])
        pieces = [-1] * p_num         # an array of (pid) dinamyc arrays

    pid = int(request.headers['pid'])    
    pieces[pid] = list(map(int, data.split(",")))
    combine()

    # Respond back with a success message
    return jsonify({"message": "Data received!", "data": data}), 200

if __name__ == '__main__':
    print("ADDER: Hello, world!")
    p_num = -1
    pieces = None

    app.run(debug=False, host="0.0.0.0", port=8080)
    print("ADDER: Goodbye!", flush=True)
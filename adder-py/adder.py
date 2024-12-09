from flask import Flask, request, jsonify
import time
from functools import wraps

app = Flask(__name__)

def time_it(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        start_time = time.time()
        result = func(*args, **kwargs)
        end_time = time.time()
        execution_time = (end_time - start_time) * 1000
        print(f"time for running function '{func.__name__}': {execution_time:.6f} milliseconds")
        return result, execution_time  # Возвращаем результат функции и время выполнения
    return wrapper

@time_it
def combine():
    global p_num
    global pieces

    print("ADDER: Combining...", flush=True)

    Nan = False
    for i in range(0, p_num):
        if pieces[i] == -1:
            Nan = True
        # print("ADDER: combine check: ", pieces[i] == -1, flush=True)

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
    global spent_time

    # print("ADDER: Got post request", flush=True)
    data = request.data.decode("utf-8")
    # print("ADDER: Received data:", data[:50], flush=True)

    if (p_num == -1):
        p_num = int(request.headers['p-num'])
        pieces = [-1] * p_num         # an array of (p_num) dinamyc arrays

    pid = int(request.headers['pid'])
    spent_time += float(request.headers['time-spent'])    
    pieces[pid] = list(map(int, data.split(",")))
    comb_res, local_time = combine()
    # print(f"comb_res = {comb_res}, local_time = {local_time}", flush=True)
    if (0 == comb_res):
        print(f"\n\nADDER:\tspent time = {spent_time / p_num}\n\n\n", flush=True)

    # Respond back with a success message
    return jsonify({"message": "Data received!", "data": data}), 200
    # return

if __name__ == '__main__':
    print("ADDER: Hello, world!", flush=True)
    p_num = -1
    pieces = None
    spent_time = 0.0

    app.run(debug=False, host="0.0.0.0", port=8080)
    print("ADDER: Goodbye!", flush=True)
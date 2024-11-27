FROM python:3

# Set the working directory inside the container
WORKDIR /usr/src/app

RUN pip install --no-cache-dir flask
# RUN apt-get update && \
#     apt install -y python-flask

# Copy the adder.py file to the working directory
COPY . .

# Set the entry point for the container
CMD [ "python", "./adder.py" ]
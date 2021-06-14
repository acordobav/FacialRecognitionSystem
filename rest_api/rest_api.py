import json, base64
from functools import wraps
from flask import Flask, request, jsonify, Response
#from flask_cors import CORS, cross_origin

app = Flask(__name__)
app.config["DEBUG"] = True
#CORS(app)

photos = []

def config_response(response):

    response.headers.add('Access-Control-Allow-Origin', '*')
    response.headers.add('Content-Type', 'application/json, text/plain, */*')
    response.headers.add('Access-Control-Allow-Methods', "GET, PUT, POST, DELETE, OPTIONS")
    response.headers.add('Access-Control-Allow-Headers', 'Content-Type, Authorization')
    return response

@app.route('/user', methods=['PUT'])
def put_user():

    user_metadata = request.get_json()

    file = open("userjson.txt", "w")
    file.write(json.dumps(user_metadata))
    file.close()

    response = jsonify(message="Usuario registrado")
    #response = config_response(response)
    response.headers.add('Access-Control-Allow-Origin', '*')
    response.headers.add('Content-Type', 'application/json, text/plain, */*')
    response.headers.add('Access-Control-Allow-Methods', "GET, PUT, POST, DELETE, OPTIONS")
    response.headers.add('Access-Control-Allow-Headers', 'Content-Type, Authorization')
    return response

@app.route('/user', methods=['POST'])
def post_user():

    user_metadata = request.get_json()
    request_email = user_metadata['email']
    request_password = user_metadata['password']
    with open("userjson.txt") as file:
        user_string = file.read()
        user_json = json.loads(user_string)
        file.close()
    file_name = user_json['name']
    file_email = user_json['email']
    file_password = user_json['password']
    if ((request_email == file_email ) & 
        (request_password == file_password)):
        token = request_email + request_password
        token_bytes = token.encode('ascii')
        token_base64_bytes = base64.b64encode(token_bytes)
        token_base64 = token_base64_bytes.decode('ascii')
        response = {"name": file_name, "email": request_email, "password": request_password, "token": token_base64}

    return jsonify(response)

@app.route('/photo', methods=['POST'])
def post_photo():

    photo_metadata = request.get_json()
    photos.append(photo_metadata)
    response = jsonify(message="Foto enviada")
    response = config_response(response)
    return response

@app.route('/photo', methods=['GET'])
def get_photos():

    return jsonify(photos)

@app.route('/photo/<string:name>', methods=['DELETE'])
def delete_photo(name):
    for photo in photos:
        if photo['name'] == name:
            photos.remove(photo)
    return '', 204

@app.route('/settings', methods=['POST'])
def post_settings():

    settings = request.get_json()
    print(settings['maxFaceDist'])
    print(settings['knownMaxFace'])
    return '', 204

if __name__ == "__main__":
    app.run()
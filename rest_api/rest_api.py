import json, base64
from flask import Flask, request, jsonify
from flask_cors import CORS, cross_origin

app = Flask(__name__)
app.config['CORS_HEADERS'] = 'Content-Type'

cors = CORS(app, resources={r"/user": {"origins": "http://localhost:8100"}})
cors = CORS(app, resources={r"/photo": {"origins": "http://localhost:8100"}})
cors = CORS(app, resources={r"/settings": {"origins": "http://localhost:8100"}})

photos = []

@app.route('/user', methods=['PUT'])
@cross_origin(origin='localhost',headers=['Content- Type','Authorization'])
def put_user():

    user_metadata = request.get_json()

    file = open("userjson.txt", "w")
    file.write(json.dumps(user_metadata))
    file.close()

    response = ''
    response.headers.add('Access-Control-Allow-Origin', '*')
    return response, 204

@app.route('/user', methods=['POST'])
@cross_origin(origin='localhost',headers=['Content- Type','Authorization'])
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
        response = json.dumps(response)
        print(response)
        #response = json.loads(response)

    return response, 204

@app.route('/photo', methods=['POST'])
@cross_origin(origin='localhost',headers=['Content- Type','Authorization'])
def post_photo():

    photo_metadata = request.get_json()
    photos.append(photo_metadata)
    return '', 204

@app.route('/photo', methods=['GET'])
@cross_origin(origin='localhost',headers=['Content- Type','Authorization'])
def get_photos():

    return jsonify(photos)

@app.route('/photo/<string:name>', methods=['DELETE'])
@cross_origin(origin='localhost',headers=['Content- Type','Authorization'])
def delete_photo(name):
    for photo in photos:
        if photo['name'] == name:
            photos.remove(photo)
    return '', 204

@app.route('/settings', methods=['POST'])
@cross_origin(origin='localhost',headers=['Content- Type','Authorization'])
def post_settings():

    settings = request.get_json()
    print(settings['maxFaceDist'])
    print(settings['knownMaxFace'])
    return '', 204

if __name__ == "__main__":
    app.run()
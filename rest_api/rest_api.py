import json, base64, os
from functools import wraps
from flask import Flask, config, request, jsonify
from flask_cors import CORS, cross_origin

app = Flask(__name__)
app.config["DEBUG"] = True
CORS(app)


def config_response(response):

    response.headers.add("Access-Control-Allow-Headers", "*")
    response.headers.add("Access-Control-Allow-Methods", "*")
    return response

# Metodo de options
@app.route('/', methods=['OPTIONS'])
@cross_origin()
def options():
    
    response = config_response('')
    return response

# Metodo para registrar un usuario
@app.route('/user', methods=['PUT'])
@cross_origin()
def put_user():

    user_metadata = request.get_json()

    file = open("userjson.txt", "w")
    file.write(json.dumps(user_metadata))
    file.close()

    response = jsonify(message="Usuario registrado")
    response = config_response(response)
    return response

# Metodo para iniciar sesion de un usuario
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

        response = jsonify(response)
        response = config_response(response)
        return response
    else:
        return '', 401

# Metodo para agregar una foto
@app.route('/photo', methods=['POST'])
def post_photo():

    photo_metadata = request.get_json()
    dir_name = "Database"
    if not os.path.exists(dir_name):
        os.mkdir(dir_name)
        print("Directory created")
    else:
        print("Directory already exists")

    image_string = photo_metadata['webviewPath']
    image_base64 = image_string.split(",")[1]
    image_data = base64.b64decode(image_base64)
    #image_data = base64.b64decode(image_string)
    filename = dir_name + "/" + photo_metadata['filepath']
    print(filename)
    with open(filename, 'wb') as file:
        file.write(image_data)


    #photos.append(photo_metadata)
    response = jsonify(message="Foto enviada")
    response = config_response(response)
    return response

# Metodo para obtener las fotos
@app.route('/photo', methods=['GET'])
def get_photos():

    photos = []
    dir_list = os.listdir("Database")
    print(dir_list)
    for photo in dir_list:
        with open("Database/" + photo, 'rb') as file:
            photo_bytes = file.read()
            photo_base64 = base64.b64encode(photo_bytes)
            #photo_base64 = "data:image/png;base64," + str(photo_base64)[2:]
            photo_base64 = str(photo_base64)[2:]
            #print(photo_base64)
            photo_metadata = {"filepath": photo, "webviewPath": photo_base64}
            photos.append(photo_metadata)

    response = jsonify(photos)
    response = config_response(response)
    return response

# Metodo para eliminar una foto
@app.route('/photo/<string:name>', methods=['DELETE'])
def delete_photo(name):


    #for photo in photos:
    #    if photo['name'] == name:
    #        photos.remove(photo)

    filename = "Database" + "/" + name
    if os.path.exists(filename):
        os.remove(filename)
    else:
        print("File does not exist")

    response = jsonify(message="Foto eliminada")
    response = config_response(response)
    return response

# Metodo para asignar variables de configuracion
@app.route('/settings', methods=['POST'])
def post_settings():

    settings = request.get_json()
    if not os.path.exists("e0.dat"):
        os.mknod("e0.dat")

    e0 = str(settings['knownMaxFace']).encode('ascii')
    with open("e0.dat", 'wb') as file:
        file.write(e0)

    if not os.path.exists("e1.dat"):
        os.mknod("e1.dat")

    e1 = str(settings['maxFaceDist']).encode('ascii')
    with open("e1.dat", 'wb') as file:
        file.write(e1)

    response = jsonify(message="Variables registradas")
    response = config_response(response)
    return response

if __name__ == "__main__":
    app.run()
import json, base64, os
from flask import Flask, config, request, jsonify
from flask_cors import CORS, cross_origin

from PIL import Image

app = Flask(__name__)
app.config["DEBUG"] = True
CORS(app)


# Image database directory path
dir_name = "/home/Database"


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
    global dir_name
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

    image = Image.open(filename)
    print(image.size)
    if (image.size[0] > image.size[1]):
        image = image.transpose(Image.ROTATE_90)

    image = image.crop((0,457,1920,2794))
    image = image.resize((92,112))
    print(image.size)
    image.save(filename)

    #photos.append(photo_metadata)
    response = jsonify(message="Foto enviada")
    response = config_response(response)
    return response

# Metodo para obtener las fotos
@app.route('/photo', methods=['GET'])
def get_photos():

    global dir_name

    photos = []
    dir_list = os.listdir(dir_name)
    print(dir_list)
    for photo in dir_list:
        with open(dir_name + "/" + photo, 'rb') as file:
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

    global dir_name
    filename = dir_name + "/" + name
    if os.path.exists(filename):
        os.remove(filename)
    else:
        print("File does not exist")

    train_algorithm()

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


"""
Metodo para iniciar el entrenamiento del algoritmo de reconocimiento
"""
def train_algorithm():
    global dir_name
    command = "./facial_recognition -a " + dir_name
    os.system(command)

if __name__ == "__main__":
    app.run()
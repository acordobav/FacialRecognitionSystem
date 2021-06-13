import json
from flask import Flask, request, jsonify

app = Flask(__name__)


photos = []

@app.route('/user', methods=['PUT'])
def put_user():

    user_metadata = request.get_json()

    return '', 204

@app.route('/user', methods=['POST'])
def post_user():

    user_metadata = request.get_json()

    return '', 204

@app.route('/photo', methods=['POST'])
def post_photo():

    photo_metadata = request.get_json()
    photos.append(photo_metadata)
    return '', 204

@app.route('/photo', methods=['GET'])
def get_photos():

    return jsonify(photos)

@app.route('/photo_delete', methods=['POST'])
def delete_photo():

    photo_metadata = request.get_json()
    name = photo_metadata[0]['name']
    for photo in photos:
        if photo[0]['name'] == name:
            photos.remove(photo)
    return '', 204

@app.route('/settings', methods=['POST'])
def post_settings():

    settings = request.get_json()
    max_face_distance = settings[0]['maxFaceDist']
    known_max_face_distance = settings[0]['knownMaxFace']
    print(max_face_distance)
    print(known_max_face_distance)
    return '', 204

if __name__ == "__main__":
    app.run()
# Create your views here.
import time
from typing import List
from django.http import HttpResponse
import json
from cross_mode_face.pre_process import process_img
from cross_mode_face.project_model.ICMR import Solver
import io
from base64 import encodebytes
import onnxruntime
from PIL import Image
import numpy as np
import os
os.environ["KMP_DUPLICATE_LIB_OK"]="TRUE"

def index(request):
    q = request.GET.get('q')
    print(q)
    book_list = [
        {'id': 1, 'name': 'ptyhon'},
        {'id': 2, 'name': 'go'},
    ]
    response = HttpResponse(json.dumps(book_list), content_type='application/json')
    response['Access-Control-Allow-Origin'] = '*'
    return response

class confg(object):
    def __init__(self, query):
        self.dataset = 'celebA'
        self.query = query
        self.epoch = 50
        self.hash_lens = 128
        self.device = 'cuda:0'
        self.task = 1


def get_response_image(image_path):
    pil_img = Image.open(image_path, mode='r') # reads the PIL image
    byte_arr = io.BytesIO()
    pil_img.save(byte_arr, format='PNG') # convert the PIL image to byte array
    encoded_img = encodebytes(byte_arr.getvalue()).decode('ascii') # encode as base64
    return encoded_img

query = 'a man with glasses.'
config = confg(query)
solver = Solver(config)

img_model_path = r'cross_mode_face\Image_Encoder_2.onnx'

# 创建ONNX Runtime的推理会话
ort_session = onnxruntime.InferenceSession(img_model_path)

def T2I(request):
    start = time.time()
    query = request.GET.get('query')
    print(query)
    if query == None:
        query = 'a man with glasses.'
    config = confg(query)
    imgs = solver.inference(config.query)
    end = time.time()
    print(f'the time of retrieval: {end - start:.2f} s')

    start = time.time()
    data = [get_response_image(img) for img in imgs]
    response = HttpResponse(json.dumps({'results':data}), content_type='application/json')
    response['Access-Control-Allow-Origin'] = '*'
    end = time.time()
    print(f'the time of showing images: {end - start:.2f} s')
    return response


def getimageFeatures(img_path : str ) -> List[float]:
    
    input_data = process_img(img_path).numpy()
    input_data = np.expand_dims(input_data, axis=0)
    out_put: np.ndarray = ort_session.run(None, {'onnx::Cast_0': input_data})
    out_put = np.sign(out_put)[0]
    out_put[out_put==-1] = 0
    out_put = out_put.astype(np.uint8)
    out_put = out_put.tolist()[0]
    return out_put


def faceFeature(request: HttpResponse) -> HttpResponse: # 返回图片的哈希码
    file_obj = request.FILES.get('file', None)
    if file_obj is not None:
        with open('test.png', 'wb') as file:
            file.write(file_obj.read())
        print(type(file_obj))
        out_put = getimageFeatures(img_path='test.png')
        response = HttpResponse(json.dumps({'results':out_put}), content_type='application/json')
        response['Access-Control-Allow-Origin'] = '*'
        return response

def igetfaceFeature(request: HttpResponse) -> HttpResponse: # 返回十张图片 与T2I函数返回的形式一样
    file_obj = request.FILES.get('file', None)
    print(type(file_obj))
    if file_obj is not None:
        with open('test.png', 'wb') as file:
            file.write(file_obj.read())
        imgs = solver.inference_img(img_path='test.png')
        data = [get_response_image(img) for img in imgs]
        response = HttpResponse(json.dumps({'results':data}), content_type='application/json')
        response['Access-Control-Allow-Origin'] = '*'
        return response
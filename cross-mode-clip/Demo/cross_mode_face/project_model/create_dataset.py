import os
import torch
import clip
import pickle

import pandas as pd
import numpy as np
from PIL import Image
from tqdm import tqdm


def read_label(path):
    #读取txt文件内的内容
    with open(path, 'r') as f:
        content = f.readlines()
    content = content[2:] #删去前两行无效信息
    content = [x.strip('\n') for x in content]
    content = [x.split(' ')[2:] for x in content]
    content = np.array(content, dtype=np.int8)
    content[content == -1] = 0
    return content

def read_image(path):
    #读取文件夹内的图像
    device = "cuda" if torch.cuda.is_available() else "cpu"
    model, preprocess = clip.load("ViT-B/32", device=device)
    image_list = [str(i) +'.jpg' for i in range(30000)]
    img = []
    batch = 100
    for i in range(len(image_list)):
        image_list[i] = os.path.join(path, image_list[i])
    for i in tqdm(range(len(image_list)//batch)):
        tmp = [Image.open(image_list[x]) for x in range(i*batch, min((i+1)*batch, len(image_list)))]
        image_x = [preprocess(x) for x in tmp]
        image_x = torch.stack(image_x).to(device)
        with torch.no_grad():
            image_features = model.encode_image(image_x).detach().cpu().numpy().astype(np.float64)
        img.extend(image_features)
        for x in tmp:
            x.close()
    return np.array(img, dtype=np.float64)

def read_text(path):
    #读取txt文件内的内容
    device = "cuda" if torch.cuda.is_available() else "cpu"
    model, preprocess = clip.load("ViT-B/32", device=device)
    text_list = [str(i) +'.txt' for i in range(30000)]
    text = []
    text_features = []
    for i in range(len(text_list)):
        text_list[i] = os.path.join(path, text_list[i])
    for t in text_list:
        with open(t, 'r') as f:
            content = f.readlines()
        text.append(content)
    #text:[30000,10,words]
    with torch.no_grad():
        for t in tqdm(text):
            te = torch.cat([clip.tokenize(x.strip('\n')) for x in t]).to(device)
            features = model.encode_text(te).detach().cpu().numpy().astype(np.float64)
            text_features.append(features)
    return np.array(text_features, dtype=np.float64)

if __name__ == "__main__":
    np.random.seed(3443)
    idx = np.arange(30000)
    np.random.shuffle(idx)
    
    idx_re = idx[2000:]
    image_list = [str(i) +'.jpg' for i in range(30000)]
    dic = {"idx": idx_re, "image_list": image_list}

    with open("dic.pkl", "wb") as f:
        pickle.dump(dic, f)
        
    anno_path = "C:/Users/Senmo/Downloads/Compressed/CelebAMask-HQ/CelebAMask-HQ/CelebAMask-HQ-attribute-anno.txt"
    label = read_label(anno_path) #[30000, 40], type=np.int8

    image_path = "C:/Users/Senmo/Downloads/Compressed/CelebAMask-HQ/CelebAMask-HQ/CelebA-HQ-img"
    image = read_image(image_path) #[30000, 512], type=np.float64

    text_path = "C:/Users/Senmo/Downloads/Compressed/CelebAMask-HQ/CelebAMask-HQ/celeba-caption"
    text = read_text(text_path) #[30000, 10, 512], type=np.float64


    label = label[idx]
    image = image[idx]
    text = text[idx]


    query_labels = label[:2000]
    query_images = image[:2000]
    query_texts = text[:2000]

    train_labels = label[2000:7000]
    train_images = image[2000:7000]
    train_texts = text[2000:7000]

    retrieval_labels = label[2000:]
    retrieval_images = image[2000:]
    retrieval_texts = text[2000:]

    train = {'text': train_texts, 'image': train_images, 'label': train_labels}
    query = {'text': query_texts, 'image': query_images, 'label': query_labels}
    retrieval = {'text': retrieval_texts, 'image': retrieval_images, 'label': retrieval_labels}

    with open('Dataset/celebA/train.pkl', 'wb') as f:
        pickle.dump(train, f)
    with open('Dataset/celebA/query.pkl', 'wb') as f:
        pickle.dump(query, f)
    with open('Dataset/celebA/retrieval.pkl', 'wb') as f:
        pickle.dump(retrieval, f)

    print("Finish!")

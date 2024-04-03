import pickle


with open('Dataset/celebA/train.pkl', 'rb') as f_pkl:
    data = pickle.load(f_pkl)
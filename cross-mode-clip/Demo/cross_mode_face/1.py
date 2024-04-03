import onnxruntime
import onnx
import numpy as np
import torch.onnx

from pre_process import process_img

# 指定ONNX模型文件路径
img_model_path = 'F:\Git\cross-mode-clip\Demo\cross_mode_face\Image_Encoder_2.onnx'

# 创建ONNX Runtime的推理会话
ort_session = onnxruntime.InferenceSession(img_model_path)

# 准备输入数据（这里假设输入数据是一个NumPy数组）
# input_data = np.random.rand(1, 3, 224, 224).astype(np.float32)
# input_data = torch.randn(1, 3, 224, 224)
input_data = process_img(r'cross_mode_face\test.jpg').numpy()
input_data = np.expand_dims(input_data, axis=0)

# 进行推理
output = ort_session.run(None, {'onnx::Cast_0': input_data})

# 输出推理结果
print(output[0].tolist()[0])



# text_model_path = 'Text_Encoder_2.onnx'

# # 创建ONNX Runtime的推理会话
# ort_session = onnxruntime.InferenceSession(text_model_path)

# # 准备输入数据（这里假设输入数据是一个NumPy数组）
# # input_data = np.random.rand(1, 77).astype(np.float32)

# # 进行推理
# # output = ort_session.run(None, {'onnx::Cast_0': input_data})

# # 输出推理结果
# # print(output)

# 打印模型的所有输入节点名称
# input_names = [input.name for input in ort_session.get_inputs()]
# print("Model input names:", input_names)



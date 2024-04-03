# Demo文件夹为 django 后端文件夹
在cross-mode-clip目录下打开终端

  ```bash
  python -m venv ./python_env  //创建虚拟环境
  cd python_env source ./bin/activate  //激活环境
  cd ..
  cd Demo
  cd cross_mode_face
  pip3 install torch torchvision torchaudio
  pip install django==3.0
  pip install -r requirements.txt
  
  cd ..
  python manage.py runserver   //启动后端
  ```

# Demofront 文件夹为 Vue 前端文件夹

1. 下载node.js
cmd下
2. npm config set registry https://registry.npm.taobao.org
3. npm install vue webpack webpack-cli vue-cli -g     
4. npm install --save axios@1.5.0 vue-axios
cd Demofront
5. npm install
6. npm run dev	




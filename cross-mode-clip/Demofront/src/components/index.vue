<template>
<div class="index">
    <div class="top">跨模态人脸检索Demo</div>
    <div class="topdiv">
        <div style="position: absolute;
            width: 300px;
            height: 300px;
            left: 50%;
            top: 50%;
            transform: translate(-50%,-50%);">
            <el-carousel indicator-position="outside">
                <el-carousel-item v-for="(item, index) in imgList" :key="index">
                    <img :src="item" class="image" style="width: 300px;height: 300px;">
                </el-carousel-item>
            </el-carousel>
        </div>


    </div>
    <div class="middiv">
        <div style="position: absolute;
            width: 500px;
            height: 100px;
            left: 50%;
            top: 50%;
            transform: translate(-50%,-50%);" v-if="this.value == 'false'">
            <el-input
            type="textarea"
            :autosize="{ minRows: 4, maxRows: 8}"
            placeholder="请输入内容"
            v-model="textarea2">
            </el-input>
        </div>
        <div style="position: absolute;
            left: 50%;
            top: 50%;
            transform: translate(-50%,-50%);" v-else>
      <el-upload
        action=""
        :auto-upload="false"
        list-type="picture-card"
        :before-upload="beforeAvatarUpload"
        :on-change="changeimg">

            <img class="el-upload-list__item-thumbnail"
                :src="img_url" alt="">

        <span 
          class="el-upload-list__item-preview"
          @click="handlePictureCardPreview(img_url)"
        >
          <i class="el-icon-zoom-in"></i>
        </span>
        <span
          v-if="!disabled"
          class="el-upload-list__item-delete"
          @click="handleDownload(img_url)"
        >
          <i class="el-icon-download"></i>
        </span>
        <span
          v-if="!disabled"
          class="el-upload-list__item-delete"
          @click="handleRemove(img_url)"
        >
          <i class="el-icon-delete"></i>
        </span>
      
            
            
        </el-upload>
        </div>
    </div>
    <div class="bottomdiv">
        <!-- <el-input v-model="input" placeholder="请输入文本" style="width: 60%;left: 10%;"></el-input> -->
        <div style="position: absolute;
            left: 50%;
            top: 20%;
            transform: translate(-50%,-50%);">
            <el-switch
                v-model="value"
                active-text="图片检索文本"
                inactive-text="文本检索图片"
                active-value="ture"
                inactive-value="false"
                >
            </el-switch >
        </div>

        <el-button round style="position: absolute;
            left: 50%;
            top: 50%;
            transform: translate(-50%,-50%);"
            @click='TgetData'
            v-if="this.value == 'false'"
            >检索</el-button>

        <el-button round style="position: absolute;
            left: 50%;
            top: 50%;
            transform: translate(-50%,-50%);"
            @click='IgetData(file)'
            v-else >检索</el-button>
    </div>
</div>
</template>

<script>
export default {
    name: 'index',
    data () {
        return {
          file:'',
            dialogImageUrl: '',
            dialogVisible: false,
            disabled: false,
        urll:"http://127.0.0.1:8000/igetfaceFeature",
        img_url:"",
          imgSrc: "",
          visible: false,//弹窗
          loading: false,//上传按钮加载
          open: false,//控制摄像头开关
          thisVideo: null,
          thisContext: null,
          thisCancas: null,
          videoWidth: 500,
          videoHeight: 400,
        input: '',
        value: "true",
        textarea2:'',
        textarea1:'',
        imgList: [
                '../../static/img/11.JPG',
                '../../static/img/11.JPG',
                '../../static/img/11.JPG',
                '../../static/img/11.JPG'
            ],
        }
    },
    methods: {
        changeimg(file){
            this.file = file
        },
        handleRemove(file) {
        console.log(file);
      },
      handlePictureCardPreview(file) {
        this.dialogImageUrl = file.url;
        this.dialogVisible = true;
      },
      handleDownload(file) {
        console.log(file);
      },
        TgetData() {
            // this.imgList = []
            if (this.textarea2 != null){
                this.$axios.get('http://127.0.0.1:8000/?'+'query='+this.textarea2).then(res => {
                    console.log(res.data)
                    var tempArr = []
                    if (res.data.results){
                        for(let i = 0;  i < res.data.results.length; i ++) {
                        tempArr[i] = `data:image/png;base64,${res.data.results[i]}`
                        //   let tempOb = {}
                        //   tempOb.decodedImage = decodedImage
                        //   tempArr[i] = tempOb
                        }
                        this.imgList = tempArr
                    }
                })
        }
        },  

    IgetData (file) {
        var formdata = new FormData()
        formdata.append('file', file.raw)
        this.$axios.post('http://127.0.0.1:8000/igetfaceFeature',formdata).then(rsp=>{
            console.log(rsp.data)
            var tempArr = []
            if (rsp.data.results){
                        for(let i = 0;  i < rsp.data.results.length; i ++) {
                        tempArr[i] = `data:image/png;base64,${rsp.data.results[i]}`
                        //   let tempOb = {}
                        //   tempOb.decodedImage = decodedImage
                        //   tempArr[i] = tempOb
                        }
                        this.imgList = tempArr
                    }
        })
      },
        //base64转成文件后上传
        onUpload() {
          if (this.imgSrc) {
            const file = this.imgSrc; // 把整个base64给file
            const time = (new Date()).valueOf();//生成时间戳
            const name = time + ".png"; // 定义文件名字（例如：abc.png ， cover.png）
            const conversions = this.dataURLtoFile(file, name); // 调用base64转图片方法
            let data =new FormData();
            data.append('file', conversions);
            this.$axios.post('http://127.0.0.1:8000/igetfaceFeature',data).then(rsp=>{
            console.log(rsp)
        })
          }
        },
        handleAvatarSuccess (res, file) {
        // this.img_url = URL.createObjectURL(file.raw)
        this.file_img = file.raw
        var formdata = new FormData()
        formdata.append('file', file.raw)
        this.$axios.post('http://127.0.0.1:8000/igetfaceFeature',formdata).then(rsp=>{
            console.log(rsp.data)
            var tempArr = []
                    if (rsp.data.results){
                        for(let i = 0;  i < rsp.data.results.length; i ++) {
                        tempArr[i] = `data:image/png;base64,${rsp.data.results[i]}`
                        //   let tempOb = {}
                        //   tempOb.decodedImage = decodedImage
                        //   tempArr[i] = tempOb
                        }
                        this.imgList = tempArr
                    }
        })
      },
        //文件上传到前端系统前，会调用的钩子函数，主要对上传的文件进行限制
        beforeAvatarUpload (file) {
        const isLt2M = file.size / 1024 / 1024 < 2
        this.$message.warning(('文件上传中'))
        if (!isLt2M) {
          this.$message.error('上传头像图片大小不能超过 2MB!')
        }
        return isLt2M
      },
        // 调用摄像头权限
        getCompetence() {
          //必须在model中render后才可获取到dom节点,直接获取无法获取到model中的dom节点
          this.$nextTick(() => {
            const _this = this;
            this.open = false;//切换成关闭摄像头
            this.thisCancas = document.getElementById('canvasCamera');
            this.thisContext = this.thisCancas.getContext('2d');
            this.thisVideo = document.getElementById('videoCamera');
            // 旧版本浏览器可能根本不支持mediaDevices，我们首先设置一个空对象
            if (navigator.mediaDevices === undefined) {
              navigator.mediaDevices = {}
            }
            // 一些浏览器实现了部分mediaDevices，我们不能只分配一个对象
            // 使用getUserMedia，因为它会覆盖现有的属性。
            // 这里，如果缺少getUserMedia属性，就添加它。
            if (navigator.mediaDevices.getUserMedia === undefined) {
              navigator.mediaDevices.getUserMedia = function (constraints) {
                // 首先获取现存的getUserMedia(如果存在)
                let getUserMedia = navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.getUserMedia;
                // 有些浏览器不支持，会返回错误信息
                // 保持接口一致
                if (!getUserMedia) {
                  return Promise.reject(new Error('getUserMedia is not implemented in this browser'))
                }
                // 否则，使用Promise将调用包装到旧的navigator.getUserMedia
                return new Promise(function (resolve, reject) {
                  getUserMedia.call(navigator, constraints, resolve, reject)
                })
              }
            }
            const constraints = {
              audio: false,
              video: {width: _this.videoWidth, height: _this.videoHeight, transform: 'scaleX(-1)'}
            };
            navigator.mediaDevices.getUserMedia(constraints).then(function (stream) {
              // 旧的浏览器可能没有srcObject
              if ('srcObject' in _this.thisVideo) {
                _this.thisVideo.srcObject = stream
              } else {
                // 避免在新的浏览器中使用它，因为它正在被弃用。
                _this.thisVideo.src = window.URL.createObjectURL(stream)
              }
              _this.thisVideo.onloadedmetadata = function (e) {
                _this.thisVideo.play()
              }
            }).catch(err => {
              this.$notify({
                title: '警告',
                message: '没有开启摄像头权限或浏览器版本不兼容.',
                type: 'warning'
              });
            });
          });
        },
        //绘制图片
        drawImage() {
          // 点击，canvas画图
          this.thisContext.drawImage(this.thisVideo, 0, 0, this.videoWidth, this.videoHeight);
          // 获取图片base64链接
          this.imgSrc = this.thisCancas.toDataURL('image/png');
        },
        //base64转文件
        dataURLtoFile(dataurl, filename) {
          let arr = dataurl.split(',');
          let mime = arr[0].match(/:(.*?);/)[1];
          let bstr = atob(arr[1]);
          let n = bstr.length;
          let u8arr = new Uint8Array(n);
          while (n--) {
            u8arr[n] = bstr.charCodeAt(n)
          }
          return new File([u8arr], filename, {type: mime})
        },
        //清空画布
        clearCanvas(id) {
          let c = document.getElementById(id);
          let cxt = c.getContext("2d");
          cxt.clearRect(0, 0, c.width, c.height);
        },
        //重置画布
        resetCanvas() {
          this.imgSrc = "";
          this.clearCanvas('canvasCamera');
        },
        //关闭摄像头
        stopNavigator() {
          if (this.thisVideo && this.thisVideo !== null) {
            this.thisVideo.srcObject.getTracks()[0].stop();
            this.open = true;//切换成打开摄像头
          }
        },
        beforeDestroy() {
        this.stopNavigator()
      },
      onTake() {
          this.visible = true;
          this.getCompetence();
        },
        onCancel() {
          this.visible = false;
          this.resetCanvas();
          this.stopNavigator();
        },





    }

}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>

.top{
    top: 2%;
    width: 60%;
    height: 8%;
    left: 50%;
    transform:translate(-50%) ;
    /* background-color: #f09797; */
    overflow: hidden;
    background-size:cover;
    position: absolute;
    font-size:50px;
    text-align: center;
}
.topdiv{
    top: 11%;
    width: 60%;
    height: 44%;
    left: 20%;
    /* background-color: #8780b9; */
    overflow: hidden;
    background-size:cover;
    position: absolute;
}
.middiv{
    top: 55%;
    width: 60%;
    height: 25%;
    left: 20%;
    /* background-color: #f09797; */
    overflow: hidden;
    background-size:cover;
    position: absolute;
}
.bottomdiv{
    top: 82%;
    width: 50%;
    height: 15%;
    left: 25%;
    /* transform:translate(-50%) ; */
    /* background-color: #f09797; */
    overflow: hidden;
    background-size:cover;
    position: absolute;
}
.index{
    width: 100%;
    height: 100%;
    /* background-color: #ebd9d9; */
    overflow: hidden;
    background-size:cover;
    position: absolute;
}
.el-carousel__item h3 {
    color: #475669;
    font-size: 18px;
    opacity: 0.75;
    line-height: 300px;
    margin: 0;
  }
  
.el-carousel__item:nth-child(2n) {
    background-color: #99a9bf;
  }
  
.el-carousel__item:nth-child(2n+1) {
    background-color: #d3dce6;
  }
.box {
display: flex;
justify-content: space-between;
}

.canvas {
border: 1px solid #e8e8e8;
}
</style>

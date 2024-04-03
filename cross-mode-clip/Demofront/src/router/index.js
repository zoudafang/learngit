import Vue from 'vue'
import Router from 'vue-router'
import index from '@/components/index'
import camera from '@/components/camera'

Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'index',
      component: index
    },
    {
      path: '/camera',
      name: 'camera',
      component: camera
    }
  ]
})

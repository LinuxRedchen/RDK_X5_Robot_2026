// pages/index/index.js
const OneNetToken = require('../../utils/onenetToken.js');
const KEY = "203a0355007a89a560794948debf58b4"
// 获取天气图标的函数
function getIconKey(weather){
  const iconWeatherMap = {
    '晴': ['晴', '少云', '晴间多云'],
    '多云': ['多云'],
    '阴': ['阴'],
    '风': [
      '有风', '平静', '微风', '和风', '清风', '强风/劲风', '疾风', '大风', 
      '烈风', '风暴', '狂爆风', '飓风', '热带风暴'
    ],
    '霾': ['霾', '中度霾', '重度霾', '严重霾'],
    '雨': [
      '阵雨', '雷阵雨', '雷阵雨并伴有冰雹', '小雨', '中雨', '大雨', '暴雨', 
      '大暴雨', '特大暴雨', '强阵雨', '强雷阵雨', '极端降雨', '毛毛雨/细雨',
      '雨', '小雨-中雨', '中雨-大雨', '大雨-暴雨', '暴雨-大暴雨', '大暴雨-特大暴暴'
    ],
    '雨夹雪': ['雨雪天气', '雨夹雪', '阵雨夹雪', '冻雨'],
    '雪': [
      '雪', '阵雪', '小雪', '中雪', '大雪', '暴雪', '小雪-中雪', '中雪-大雪', 
      '大雪-暴雪'
    ],
    '雾': ['雾', '浓雾', '强浓雾', '轻雾', '大雾', '特强浓雾'],
  };
  let iconKey = '阴'; // 默认值
  // 遍历 iconWeatherMap 查找匹配的天气类型
  for (const weatherKey in iconWeatherMap) {
    if (iconWeatherMap[weatherKey].includes(weather)) {
      iconKey = weatherKey;
      break;
    }
  }
  return iconKey;
}

Page({
  // 设备配置信息（挂载到页面实例）
  config: {
    productId: "Q782HbiNO4",    /* 产品ID */
    deviceName: "Robot",       /* 设备名 */
    deviceAccessKey: "bzdzQkwwTGNRTlhhQ0VDbnBkR1ZrRVB3WlRjTkN6ZU0=",  /* 设备密钥 */
    start_time: 0,
    end_time: 0
  },
  data: {
    title: '',
    Sensor_List:[       /* 传感器物模型数据 */
      {
        id:'Fan',
        name:'风扇状态',
        img:"../../images/Sensor/风扇图标.png",
        value:0,
        time:'08:00:00',
        unit:''
      },
      {
        id:'HotCold',
        name:'制冷制热状态',
        img:"../../images/Sensor/冷热状态图标.png",
        value:0,
        time:'08:00:00',
        unit:''
      },
      {
        id:'Humi',
        name:'柜内湿度',
        img:"../../images/Sensor/湿度图标.png",
        value:0,
        time:'08:00:00',
        unit:'%'
      },
      {
        id:'Temp',
        name:'柜内温度',
        img:"../../images/Sensor/温度图标.png",
        value:0,
        time:'08:00:00',
        unit:'℃'
      },
      {
        id:'status',
        name:'外卖柜门状态',
        img:"../../images/Sensor/外卖图标.png",
        value:0,
        time:'08:00:00',
        unit:''
      },
      
     
  
    ],
    Actuator_List:[     /* 执行器物模型控制 */
        
        
        
         
     
    ],
    settingsList: [     /* 参数物模型设置 */
    
    ],
    token: '',
    intervalId: null,
    device_status: [],        // 用于存储设备状态的数组
    activeSlider: null,
    startX: 0,
    startValue: 0,
  },

  // 在Page对象中添加格式化方法
formatTime(timestamp) {
  const date = new Date(timestamp);
  const hours = date.getHours().toString().padStart(2, '0');
  const minutes = date.getMinutes().toString().padStart(2, '0');
  const seconds = date.getSeconds().toString().padStart(2, '0');
  return `${hours}:${minutes}:${seconds}`;
},
  /**
   * 获取时间戳（最近一周）
   */
  get_timestamps() {
    const now = Date.now();
    const oneWeekAgo = now - 7 * 24 * 60 * 60 * 1000;
    return {
      start_time: oneWeekAgo,
      end_time: now
    };
  },

  /**
   * 获取设备属性（温度湿度）
   */
  getDeviceProperties() {
    if (!this.data.token) {
      console.error('Token未生成');
      wx.showToast({ title: '请先生成Token', icon: 'none' });
      return;
    }

    wx.request({
      url: `https://iot-api.heclouds.com/thingmodel/query-device-property`,
      data: {
        product_id: this.config.productId,
        device_name: this.config.deviceName
      },
      header: {
        "Authorization": this.data.token,
        "Content-Type": "application/json"
      },
      method: "GET",
      success: (res) => {
        if (res.statusCode === 200 && res.data.data) {
          console.log("物模型数据：",res);
          const dataList = res.data.data;
          dataList.forEach(item => {
            /* 获取传感器数据 */
            const index = this.data.Sensor_List.findIndex(sensor => sensor.id === item.identifier);
            if (index !== -1) {
              const key1 = `Sensor_List[${index}].time`; // 使用模板字符串动态构造路径
              const key2 = `Sensor_List[${index}].value`;
              this.setData({
                [key1]: this.formatTime(item.time), // 使用计算属性名（ES6语法）
                [key2]: item.value,
              });
            }
            /* 执行器状态同步 */
            const index1 = this.data.Actuator_List.findIndex(actuator => actuator.id === item.identifier);
            if(index1 != -1){
                if(this.data.Actuator_List[index1].synchronous == false){
                  this.data.Actuator_List[index1].synchronous = true;  //标记状态已更新
                  this.data.Actuator_List[index1].value = typeof item.value === 'string' 
                  ? item.value.toLowerCase() === 'true'
                  : Boolean(item.value);
                  /* 更新界面 */
                  const key = `Actuator_List[${index1}].value`;
                  this.setData({
                    [key]: this.data.Actuator_List[index1].value,
                  });
                }
            }
            /* 阈值信息同步 */
            const index2 = this.data.settingsList.findIndex(setting => setting.id === item.identifier);
            if(index2 != -1){
              if(this.data.settingsList[index2].synchronous == false)
              {
                this.data.settingsList[index2].synchronous = true;
                this.data.settingsList[index2].value = item.value;
                this.updateSliderItem(this.data.settingsList[index2],item.value)
              }
            }
          });
        } else {
          console.error('API返回异常:', res);
          wx.showToast({ title: '数据解析失败', icon: 'none' });
        }
      },
      fail: (err) => {
        console.error('请求失败:', err);
        wx.showToast({ title: '网络请求失败', icon: 'none' });
      }
    });
  },

  /**
   * 获取设备状态
   */
  fetchDeviceStatus() {
    if (!this.data.token) return;
    wx.request({
      url: `https://iot-api.heclouds.com/device/status-history`,
      data: {
        product_id: this.config.productId,
        device_name: this.config.deviceName,
        start_time: this.config.start_time,
        end_time: this.config.end_time,
        limit: 1
      },
      header: {
        'Authorization': this.data.token,
        'Content-Type': 'application/json'
      },
      method: "GET",
      success: (res) => {
        if (res.data) {
          console.log("设备状态：",res);
          this.setData({ device_status: res });
        }
      },
      fail: (err) => {
        console.error("状态请求失败:", err);
      }
    });
  },

  /**
   * 页面加载
   */
  onLoad() {
    this.updateAllPositions();// 初始化位置计算
    this.getLocation();
    try {
      const token = OneNetToken.generateDefaultToken(
        this.config.productId,
        this.config.deviceName,
        this.config.deviceAccessKey,
        3600000 // 有效期1小时（单位秒）
      );
      this.setData({
        token
      });
    } catch (error) {
      console.error('初始化失败:', error);
      wx.showToast({ title: '初始化失败', icon: 'none' });
    }
    const { start_time, end_time } = this.get_timestamps(); // 获取时间戳
    this.config.start_time = start_time;
    this.config.end_time = end_time;
    this.getDeviceProperties(); // 先获取一次设备数据，更新页面
    this.fetchDeviceStatus(); // 获取设备在线情况
    setInterval(() => {
      const { start_time, end_time } = this.get_timestamps();
      this.config.start_time = start_time;
      this.config.end_time = end_time;
      this.fetchDeviceStatus(); // 定期获取设备状态
      this.getDeviceProperties(); // 定期获取设备数据
    }, 3000); // 推荐每3000毫秒更新一次，根据实际数据刷新情况调整
  },
  // 更新所有滑块位置
  updateAllPositions() {
    const newList = this.data.settingsList.map(item => {
      return this.calculatePosition(item);
    });
    this.setData({ settingsList: newList });
  },

  // 计算单个滑块位置
  calculatePosition(item) {
    const percent = (item.value - item.min) / (item.max - item.min) * 100;
    return {
      ...item,
      percent: `${percent}%`,
      position: `calc(${percent}% - 20rpx)`
    };
  },
  /**
   * 更新进度条的值并刷新界面
   * @param {Object} item - settingsList 的某一项
   * @param {number} newValue - 新的值
   */
  updateSliderItem(item, newValue) {
    // 1. 确保新值在合法范围内（min <= newValue <= max）
    newValue = Math.max(item.min, Math.min(item.max, newValue));

    // 2. 计算新的 percent 和 position
    const updatedItem = this.calculatePosition({
      ...item,
      value: newValue,
    });

    // 3. 找到该项在 settingsList 中的索引
    const index = this.data.settingsList.findIndex(i => i.id === item.id);
    if (index === -1) return; // 如果没找到，直接返回

    // 4. 更新数据并刷新界面
    this.setData({
      [`settingsList[${index}]`]: updatedItem,
    });
  },
  // 触摸开始
  handleTouchStart(e) {
    const id = e.currentTarget.dataset.id;
    const item = this.data.settingsList.find(i => i.id === id);
    
    this.setData({
      activeSlider: id,
      startX: e.touches[0].pageX,
      startValue: item.value
    });
  },

  // 触摸移动
  handleTouchMove(e) {
    const { activeSlider, startX, startValue } = this.data;
    if (!activeSlider) return;

    const sliderWidth = 300; // 建议动态获取
    const deltaX = e.touches[0].pageX - startX;
    
    const newList = this.data.settingsList.map(item => {
      if (item.id === activeSlider) {
        // 计算新值
        let newValue = startValue + (deltaX / sliderWidth) * (item.max - item.min);
        newValue = Math.round(newValue / item.step) * item.step;
        newValue = Math.max(item.min, Math.min(item.max, newValue));
        // 计算新位置
        const percent = (newValue - item.min) / (item.max - item.min) * 100;
        return {
          ...item,
          value: newValue,
          percent: `${percent}%`,
          position: `calc(${percent}% - 20rpx)`
        };
      }
      return item;
    });

    this.setData({ settingsList: newList });
  },
  getLocation(){
    let that = this;  // 在函数内部保存当前的 this 引用
    wx.getLocation({
      type: 'wgs84',
      success (res) {
        console.log(res)
        const latitude = res.latitude
        const longitude = res.longitude
        const speed = res.speed
        const accuracy = res.accuracy
        wx.request({
          url: 'https://restapi.amap.com/v3/geocode/regeo?parameters',
          data:{
            key:KEY,
            location:longitude+","+latitude
          },
          success(res)
          {
            console.log(res)
            let cityCode = res.data.regeocode.addressComponent.adcode
            wx.request({
              url: 'https://restapi.amap.com/v3/weather/weatherInfo?parameters',
              data:{
                key:KEY,
                city:cityCode
              },
              success(res)
              {
                console.log(res)
                let weather_info = res.data.lives[0]
                weather_info.imageUrl = "/images/weather/"+getIconKey(weather_info.weather)+".png"
                // console.log(weather_info.imageUrl)
                that.setData({
                  weatherInfo:weather_info
                })
              }
            })
          }
        })
      }
     })
  },
  /**
   * 页面卸载
   */
  onUnload() {
    clearInterval(this.data.intervalId);
  },
  /**
   * @简要：设置设备属性
   * @参数：event - 触发事件的对象，包含自定义数据和开关状态
   * @注意：向OneNET平台发送请求，设置设备的属性值，并根据返回结果提示用户。
   * @返回值：无
   */
  onenet_set_device_property(event) {
    // 从事件对象中获取参数名和开关状态
    const param_name = event.currentTarget.dataset.param; // 从自定义数据中获取参数名
    const is_checked = event.detail.value; // 获取开关组件的状态值
    
    // 显示加载提示框，提示用户操作正在进行中
    wx.showLoading({
      title: '正在执行...', // 提示文字
      mask: true, // 显示透明蒙层，防止触摸穿透
    });
    
    // 发起网络请求
    wx.request({
      url: `https://iot-api.heclouds.com/thingmodel/set-device-property`, // 拼接API地址
      method: 'POST', // 使用POST方法
      header: {
        'Authorization': this.data.token, // 设置授权信息
        'Content-Type': 'application/json' // 建议添加内容类型头
      },
      data: {
        "product_id": this.config.productId, // 产品ID
        "device_name": this.config.deviceName, // 设备名称
        "params": {
          [param_name]: is_checked // 使用动态属性名设置参数值
        }
      },
      success: (res) => {
        console.log('OneNET属性设置请求成功，返回数据', res.data); // 打印接收到的数据

        // 隐藏加载提示框
        wx.hideLoading();
        
        // 检查响应是否成功
        if (res.data && res.data.code === 0 && res.data.data && res.data.data.code === 200) {
          // 显示成功提示框
          wx.showToast({
            title: '操作成功', // 提示文字
            icon: 'success',  // 成功图标
            duration: 1500    // 显示时长
          });
        } else {
          const index = this.data.Actuator_List.findIndex(actuator => actuator.id === param_name);
          if(index != -1){
            this.data.Actuator_List[index].value = !is_checked;
            /* 更新界面 */
            const key = `Actuator_List[${index}].value`;
            this.setData({
              [key]: this.data.Actuator_List[index].value,
            });
          }
          // 显示失败提示框，使用服务器返回的错误信息或默认提示
          wx.showToast({
            title: res.data.msg || '操作失败', 
            icon: 'none', // 不显示图标
            duration: 1500
          });
        }
      },
      fail: (err) => {
        console.log('OneNET属性设置请求失败，返回数据：', err); // 打印错误信息
        // 隐藏加载提示框
        wx.hideLoading();
        // 显示失败提示框
        wx.showToast({
          title: '操作失败', 
          icon: 'none', 
          duration: 1500
        });
      }
    });
  },
  // 保存设置
  saveSettings() {
    // 1. 准备提交数据
    const properties = {};
    this.data.settingsList.forEach(item => {
      properties[item.id] = item.value; // 使用id作为物模型标识符
    });
  
    // 2. 显示加载状态
    wx.showLoading({
      title: '提交设置中...',
      mask: true
    });
  
    // 3. 构建请求数据
    const postData = {
      product_id: this.config.productId,
      device_name: this.config.deviceName,
      params: properties
    };
  
    // 4. 发起API请求
    wx.request({
      url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
      method: 'POST',
      header: {
        'Authorization': this.data.token,
        'Content-Type': 'application/json'
      },
      data: postData,
      success: (res) => {
        wx.hideLoading();
        
        if (res.data.code === 0 && res.data.data.code === 200) {
          wx.showToast({
            title: '设置提交成功',
            icon: 'success',
            duration: 2000
          });
          console.log('服务器响应:', res.data);
        } else {
          this.handleSaveError(res.data.msg || '服务器返回异常');
        }
      },
      fail: (err) => {
        wx.hideLoading();
        this.handleSaveError('网络请求失败');
        console.error('提交失败:', err);
      }
    });
  },
  
  // 错误处理统一方法
  handleSaveError(msg) {
    wx.showToast({
      title: `提交失败: ${msg}`,
      icon: 'none',
      duration: 2500
    });
    
    // 可以在这里添加错误上报逻辑
    // this.reportError(msg);
  },
})
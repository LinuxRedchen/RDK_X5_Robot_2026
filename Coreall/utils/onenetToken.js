// utils/onenetToken.js
const CryptoJS = require('./crypto-js.min.js'); // 需要引入CryptoJS库

class OneNetToken {
  /**
   * 生成Authorization Token
   * @param {String} method - 哈希方法 (sha1/md5/sha256)
   * @param {String} res - 资源路径 (如: userid/130037)
   * @param {String} accessKey - 访问密钥
   * @param {Number} et - 有效时间(毫秒)
   * @returns {String} - 生成的Token
   */
  static generateAuthorization(method, res, accessKey, et) {
    const version = '2022-05-01';
    const expirationTime = Math.ceil((Date.now() + et) / 1000); // 计算过期时间戳
    
    // 准备签名字符串
    const stringForSignature = `${expirationTime}\n${method}\n${res}\n${version}`;
    
    // 生成签名
    const signature = this.generateSignature(stringForSignature, accessKey, method);
    
    // URL编码
    const encodedRes = encodeURIComponent(res);
    const encodedSign = encodeURIComponent(signature);
    
    return `version=${version}&res=${encodedRes}&et=${expirationTime}&method=${method}&sign=${encodedSign}`;
  }

  /**
   * 生成HMAC签名
   * @param {String} data - 要签名的数据
   * @param {String} key - 密钥
   * @param {String} method - 哈希方法
   * @returns {String} - Base64编码的签名
   */
  static generateSignature(data, key, method) {
    // 将Base64密钥转换为WordArray
    const keyBytes = CryptoJS.enc.Base64.parse(key);
    
    // 根据方法选择对应的HMAC算法
    let hmac;
    switch (method.toLowerCase()) {
      case 'sha1':
        hmac = CryptoJS.HmacSHA1(data, keyBytes);
        break;
      case 'md5':
        hmac = CryptoJS.HmacMD5(data, keyBytes);
        break;
      case 'sha256':
        hmac = CryptoJS.HmacSHA256(data, keyBytes);
        break;
      default:
        throw new Error('不支持的签名方法');
    }
    
    // 返回Base64编码的签名
    return hmac.toString(CryptoJS.enc.Base64);
  }

  /**
   * 便捷方法：生成默认Token
   * @param {String} userId - 用户ID
   * @param {String} accessKey - 访问密钥
   * @param {Number} [expiresIn=3600000] - 有效时间(毫秒)，默认1小时
   * @returns {String} - 生成的Token
   */
  static generateDefaultToken(productid, device_name,device_accessKey, expiresIn = 3600000) {
    const method = 'sha1';
    const res = "products/"+productid+"/devices/"+device_name+"";
    return this.generateAuthorization(method, res, device_accessKey, expiresIn);  
  }
}

module.exports = OneNetToken;
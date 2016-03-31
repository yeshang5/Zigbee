var mongoose = require('mongoose');
var CONSTANT = require('../constant');  //常量
var crypto = require('crypto'); //加密算法
var moment = require('moment'); //时间格式模块

function hashPW(pwd){   //hash256算法
    return crypto.createHash('sha256').update(pwd).digest('base64').toString();
}

var UserSchema = new mongoose.Schema({
    username:{
        type:String,
        unique:true         /*账号唯一性*/
    },
    usertype:{
        type:Number,
        default:CONSTANT.USER.OPER
    },
    hashed_password:String,
    createTime:String         /*注册时间*/
    //lastLogin:Date          /*最近一次登出时间*/
});

var User = mongoose.model('User',UserSchema);

//设置1个固定的管理员
var admin1 = new User({
   username:'admin1',
   usertype:CONSTANT.USER.ADMIN,
   hashed_password: hashPW('111111'),
   createTime: String(moment().format('YYYY-MM-DD HH:mm:ss'))
});

admin1.save(function(err){
    if(err){
        //console.log('amdmin exist!');
    }
});

//设置1个固定的管理员
var admin2 = new User({
    username:'admin2',
    usertype:CONSTANT.USER.ADMIN,
    hashed_password: hashPW('222222'),
    createTime: String(moment().format('YYYY-MM-DD HH:mm:ss'))
});

admin2.save(function(err){
    if(err){
        //console.log('amdmin exist!');
    }
});



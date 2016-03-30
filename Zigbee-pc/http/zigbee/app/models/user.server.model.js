var mongoose = require('mongoose');


var UserSchema = new mongoose.Schema({
    username:{
        type:String,
        unique:true         /*账号唯一性*/
    },
    usertype:{
        type:Number
    },
    hashed_password:String,
    createTime:String,        /*注册时间*/
    //lastLogin:Date          /*最近一次登出时间*/
});

var User = mongoose.model('User',UserSchema);

//设置管理员
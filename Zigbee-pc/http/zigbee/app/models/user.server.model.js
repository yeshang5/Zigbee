var mongoose = require('mongoose');

var UserSchema = new mongoose.Schema({
    username:{
        type:String,
        unique:true         /*账号唯一性*/
    },
    hashed_password:String,
    createTime:Date,        /*注册时间*/
    lastLogin:Date          /*最近一次登出时间*/
});

mongoose.model('User',UserSchema);


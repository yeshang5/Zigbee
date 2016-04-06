var mongoose = require('mongoose');
var moment = require('moment'); //时间格式模块
var CONSTANT = require('../constant');  //常量


module.exports = {
    /*登录*/
    baseSignup:function(req,res,next) {
        if (req.session.user) {           //如果登录过页面
            res.render('baseSignup',{username:req.session.username,usertype:req.session.usertype});
        } else {
            res.redirect('/');            //否则返回首页
        }
    },

    /*基站注册*/
    baseSignupAccount:function(req,res,next){

    }
};


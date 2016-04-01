var mongoose = require('mongoose');
var crypto = require('crypto'); //加密算法
var CONSTANT = require('../constant');  //常量

function hashPW(pwd){   //hash256算法
    return crypto.createHash('sha256').update(pwd).digest('base64').toString();
}


module.exports = {
    /*首页登录*/
    login: function(req,res,next){
        if(req.session.user){        //如果已经登录过页面
            res.redirect('home');    //重定向到主页
        } else{
            res.render('login');
        }
    },

    /*登录验证*/
    loginAuthen: function(req,res,next){
        var User = mongoose.model('User');    //User Model已经发布，可以直接通过名字索引
        User.findOne({username:req.body.name})
            .exec(function(err,user){
                if(!user){
                    err = 'user not find!'
                } else if(user.hashed_password === hashPW(req.body.password.toString())){
                    req.session.user = user.id;
                    req.session.username = user.username;
                    req.session.usertype = user.usertype;
                    res.redirect('home');    //重定向到主页
                } else{
                    err = 'Authentication failed!';
                }

                if(err){
                    req.session.error = err;
                    res.redirect('/');    //重定向到主页
                }
        });
    }
};
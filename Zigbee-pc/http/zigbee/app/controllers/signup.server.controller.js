var mongoose = require('mongoose');

//加密算法
var crypto = require('crypto');

//hash256算法
function hashPW(pwd){
    return crypto.createHash('sha256').update(pwd).digest('base64').toString();
}






module.exports = {

    /*登录注册页*/
    signup:function(req,res,next) {
        if (req.session.user) {           //如果登录过页面
            res.render('signup',{username:req.session.user.name});
        } else {
            res.redirect('/');            //否则返回首页
        }
    },

    /*账号注册*/
    signupAccout:function(req,res,next){

        console.log(req.body);
        //res.json({msg:"1234"});
        res.json(req.body);
    }
};

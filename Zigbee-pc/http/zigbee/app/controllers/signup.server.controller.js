var mongoose = require('mongoose');
var moment = require('moment'); //时间格式模块

//常量
var MSG = {
    USER_EXIST:1,
    USER_SUCCESS:2
};



//加密算法
var crypto = require('crypto');

//hash256算法
function hashPW(pwd){
    return crypto.createHash('sha256').update(pwd).digest('base64').toString();
}


var User = mongoose.model('User');    //User Model已经发布，可以直接通过名字索引

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
        var user = new User({
            username:req.body.name,
            hashed_password:hashPW(req.body.password),
            createTime:String(moment().format('YYYY-MM-DD HH:mm:ss'))
        });

        user.save(function(err){
            if(err){
                res.json({msg:MSG.USER_EXIST});    //注册失败，用户名已经存在
            } else{
                res.json({msg:MSG.USER_SUCCESS});  //注册成功
            }
        });
        //console.log(req.body);
        //res.json({msg:"1234"});
        //res.json(req.body);
    }
};

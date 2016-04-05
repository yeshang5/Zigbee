var mongoose = require('mongoose');
var crypto = require('crypto'); //加密算法
var CONSTANT = require('../constant');  //常量

function hashPW(pwd){   //hash256算法
    return crypto.createHash('sha256').update(pwd).digest('base64').toString();
}

var User = mongoose.model('User');    //User Model已经发布，可以直接通过名字索引

module.exports = {
    /*进入账号页*/
    account: function(req,res,next) {
        if (req.session.user) {           //如果登录过页面
            res.render('account',{username:req.session.username,usertype:req.session.usertype});
        } else {
            res.redirect('/');            //否则返回首页
        }
    },

    /*显示账号列表*/
    accountTable:function(req,res,next){
        User.find({},function(err,users){
            if(err){
                res.json({"id": 1,
                    "name": "无",
                    "level": "无"});
            }
            res.json(users);
        });
    },

    /*修改密码*/
    modifyPsw:function(req,res,next){

        function modify(){
            User.findOne({username:req.body.name})
                .exec(function(err,user){
                    if(!user){
                        err = 'user not find!'
                    } else if(user.hashed_password === hashPW(req.body.password.toString())){
                        user.hashed_password = hashPW(req.body.newPassword.toString());
                        user.save(function(error){
                            if(error){
                                err = error
                            } else{
                                res.json({msg:CONSTANT.MSG.USER_SUCCESS});
                            }
                        });
                    } else{
                        err = 'Authentication failed!';
                    }

                    if(err){
                        req.session.error = err;
                        res.json({msg:CONSTANT.MSG.USER_PSWERR});  //密码错误
                    }
            });
        }

        if(req.body.name === req.session.username){ //能修改密码的只能是自身登录的账号
            modify();
        } else{
            res.json({msg:CONSTANT.MSG.USER_AUTHFAIL});
        }
    },

    /*注销账号*/
    deletePsw: function(req,res,next){
        //console.log(req.body);
        User.findOne({username:req.body.name})
            .exec(function(err,user){
                if(!user){
                    err = 'user not find!'
                } else {
                    user.remove();
                    res.json({msg:CONSTANT.MSG.USER_SUCCESS});  //删除成功
                }
                if(err){
                    req.session.error = err;
                    res.json({msg:CONSTANT.MSG.USER_DELFAIL});  //删除失败
                }
        })
    }
};

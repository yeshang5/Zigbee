
//var mongoose = require('mongoose');

//加密算法
var crypto = require('crypto');

//hash256算法
function hashPW(pwd){
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

        //console.log(req.body);

        var user = {
            name:"admin1",
            password:hashPW("222222")
        };

        if((user.name == req.body.name) &&
           (user.password == hashPW(req.body.password.toString()))){
            //创建新的会话
            //req.session.regenerate(function(){
                req.session.user = user;
                req.session.success = 'login success!';
                res.redirect('home');    //重定向到主页
            //});
        } else{
            //req.session.regenerate(function(){
                req.session.error = 'login failed!';
            //});
            //res.json({msg:'账号密码错误!'});
            //res.send('login','账号密码错误!');          //重定向到首页
            res.redirect('/');    //重定向到主页
        }
        //res.render('home');
    }

};
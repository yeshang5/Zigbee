
module.exports = {
    /*进入主页*/
    home: function(req,res,next) {
        if (req.session.user) {           //如果登录过页面
            res.render('home',{username:req.session.username,usertype:req.session.usertype});
        } else {
            res.redirect('/');            //否则返回首页
        }
    }
};

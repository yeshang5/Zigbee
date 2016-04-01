
module.exports = {
    logout:function(req,res,next) {
        req.session.destroy(function(){  //移除会话
            res.redirect('/');           //重定向到首页
        });
    }
};
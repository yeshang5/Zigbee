
module.exports = {

    logout:function(req,res,next) {
        //移除会话
        req.session.destroy(function(){
            res.redirect('/');           //重定向到首页
        });
    }
};
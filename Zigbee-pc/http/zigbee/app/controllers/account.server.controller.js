
module.exports = {
    /*进入主页*/
    account: function(req,res,next) {
        if (req.session.user) {           //如果登录过页面
            res.render('account',{username:req.session.username});
        } else {
            res.redirect('/');            //否则返回首页
        }
    },

    accountTable:function(req,res,next){
        //console.log('accountTable!');




        res.json([{
            "id": 9,
            "name": "张三9",
            "level": "操作员",
            "comment": "无"
        }]);
    }
};

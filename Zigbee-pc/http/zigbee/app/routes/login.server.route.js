
var loginController = require('../controllers/login.server.controller');


module.exports = function(app){
    /*app.get('/save',cmdController.save);
     app.get('/find',cmdController.find);
     app.get('/removeAll',cmdController.removeAll);*/

    //登陆页
    app.route('/')
        .get(loginController.login)                 //首页登录
        .post(loginController.loginAuthen);         //登录验证
};

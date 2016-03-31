/*常量*/
module.exports = {
    //user.server.model
    USER:{
        ADMIN:1,              //管理员
        OPER:2                //操作员
    },
    //signup.server.controller
    MSG:{
        USER_EXIST:1,       //账号已经存在
        USER_SUCCESS:2,     //账号注册成功
        USER_AUTHFAIL:3    //非管理员账号权限不够
    }
};

"use strict";

angular.module("webapp")
    .service("loginService",["$http","$q",loginService]);

function loginService($http,$q){
    function handlerRequest(method, url, data){
        var defered = $q.defer();   //声明延后执行，表示要去监控后面的执行
        var config = {
            method: method,
            url: url
        };

        if("POST" === method){
            config.data = data;
            //console.log(config.data);
        } else if("GET" === method){
            config.params = data;
        }

        //$http服务是只能接受一个参数的函数，这个参数是一个对象，包含了用来生成HTTP请求的配置内容。
        //这个函数返回一个promise对象，具有success和error两个方法。
        //config包含了data，data会被转换成 ？key=value 的形式
        $http(config).success(function(data){
            //console.log(data);
            defered.resolve(data);
        }).error(function(err){
            defered.reject(err);
        });

        return defered.promise;
    }

    return{
        processForm:function(params){
            return handlerRequest('POST','/',params);
        }
    };
}




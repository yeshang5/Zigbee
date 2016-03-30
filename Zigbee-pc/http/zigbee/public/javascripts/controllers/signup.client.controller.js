"use strict";

angular.module("webapp")
    .controller('signupController',['$scope','signupService',signupController]);

//常量
var MSG = {
    USER_EXIST:1,
    USER_SUCCESS:2
};


function signupController($scope,signupService){
    $scope.formDate = {};
    $scope.alertClass = "alert alert-info";
    $scope.msg = '请妥善保管设置的密码!';

    //清除提示
    $scope.clearAlert = function(){
        $scope.alertClass = "alert alert-info";
        $scope.msg = '请妥善保管设置的密码!';
    };


    //账号注册
    $scope.signupAccount = function(){
        signupService.signupAccount($scope.formDate).then(
            function(data){
                //$scope.msg = data;
                //alert(data.name);
                if(data.msg == MSG.USER_SUCCESS){
                    $scope.alertClass = "alert alert-success";
                    $scope.msg = '用户名'+$scope.formDate.name+'注册成功!';
                    $scope.formDate.name = '';
                    $scope.formDate.password = '';
                } else{
                    $scope.alertClass = "alert alert-danger";
                    $scope.msg = '用户名'+$scope.formDate.name+'已存在，注册失败!';
                    $scope.formDate.password = '';
                }
            },
            function(err){

            }
        );
    };
}

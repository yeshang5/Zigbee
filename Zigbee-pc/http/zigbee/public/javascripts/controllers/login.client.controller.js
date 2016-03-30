"use strict";

angular.module("webapp")
    .controller('loginController',['$scope','loginService',loginController]);

function loginController($scope,loginService){
    $scope.formDate = {};
    $scope.msg = '账号不要有空格等非法字符!';

    $scope.processForm = function(){
        loginService.processForm($scope.formDate).then(
            function(data){
                $scope.msg = data;
            },
            function(err){

            }
        );
    };
}
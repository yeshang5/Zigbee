"use strict";

angular.module("webapp")
    .controller('signupController',['$scope','signupService',signupController]);

function signupController($scope,signupService){
    $scope.formDate = {};
    $scope.msg = '账号不要有空格等非法字符!';

    $scope.signupAccount = function(){

        console.log($scope.formDate);

        signupService.signupAccount($scope.formDate).then(
            function(data){
                //$scope.msg = data;
                $scope.formDate.name = '';
                $scope.formDate.password = '';

            },
            function(err){

            }
        );
    };
}

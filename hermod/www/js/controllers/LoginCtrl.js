controllerModule

.controller('LoginCtrl', function($scope, LoginService, $ionicPopup, $state, $localStorage, $http, API) {
  

    $http.get(API.url + "/api/news")
    .then(function(data){$state.go("tab.news");},
          function(res) {console.log("fail");});

    $scope.data = {};

    //console.log($localStorage.token);
 
    $scope.login = function() {
        // -- make sure it is not mocked --
        LoginService.loginUser($scope.data.username, $scope.data.password).success(function(data) {
            $state.go('tab.news');
        }).error(function() {
            var alertPopup = $ionicPopup.alert({
                title: 'Login failed!',
                template: 'Please check your credentials!'
            });
        });
    };
});
controllerModule

.controller('RegisterCtrl', function($scope, RegisterService, $ionicPopup, $state) {
    $scope.data = {};
 
    $scope.register = function() {
        var requestData = $scope.data;
        RegisterService.registerUser(requestData).success(function() {
            var alertPopup = $ionicPopup.alert({
              title: 'User registered!',
              template: 'Please verify your email.'
            });
            alertPopup.then(function(res) {
              $state.go('login');
            });
        }).error(function(error) {
            var alertPopup = $ionicPopup.alert({
              title: 'Register failed!',
              template: error
            });
        });
    };
});
angular.module('hermod.controllers', [])

.controller('ChallengesCtrl', function($scope, $http) {

  $http.get('js/challenges.json')
        .then(function(res){
          $scope.challenges = res.data;              
      });
})

.controller('ChallengeDetailCtrl', function($scope, SERVER, $stateParams, $http) {

  $scope.challengeName = $stateParams.challengeName;

  $http({
      url: SERVER.url + "api/challenge/" + $stateParams.challengeName,
  }).
  success(function(data) {
      $scope.rank = data.rank;
  });
})

.controller('LoginCtrl', function($scope, LoginService, $ionicPopup, $state) {
    $scope.data = {};
 
    $scope.login = function() {
        LoginService.loginUser($scope.data.username, $scope.data.password).success(function(data) {
            $state.go('challenges');
        }).error(function(data) {
            var alertPopup = $ionicPopup.alert({
                title: 'Login failed!',
                template: 'Please check your credentials!'
            });
        });
    };
});

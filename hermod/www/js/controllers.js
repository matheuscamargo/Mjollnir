angular.module('hermod.controllers', [])

.controller('NewsCtrl', function($scope, $http, SERVER, NewsMock) {

  $http({
      url: SERVER.url + "api/news",
  })
  .success(function(data) {
      $scope.news = data.news;
  });

  // Mock
  // $scope.news = NewsMock.getNews();

})

.controller('ChallengesCtrl', function($scope, $http) {

  $http.get('js/challenges.json')
  
  .then(function(res){
    $scope.challenges = res.data;              
  });

})

.controller('ChallengeDetailCtrl', function($scope, SERVER, $stateParams, $http, ChallengesMock) {

  $scope.challengeName = $stateParams.challengeName;

  $http({
      url: SERVER.url + "api/challenge/" + $stateParams.challengeName,
  })
  .success(function(data) {
      $scope.rank = data.rank;
  });

  // Mock
  // $scope.rank = ChallengesMock.getRank();
})

.controller('LoginCtrl', function($scope, LoginService, $ionicPopup, $state) {
    $scope.data = {};
 
    $scope.login = function() {
        // -- make sure it is not mocked --
        LoginService.autoLogin($scope.data.username, $scope.data.password).success(function(data) {
            $state.go('tab.news');
        }).error(function() {
            var alertPopup = $ionicPopup.alert({
                title: 'Login failed!',
                template: 'Please check your credentials!'
            });
        });
    };
})

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

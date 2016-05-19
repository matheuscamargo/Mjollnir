angular.module('hermod.controllers', [])

.controller('NewsCtrl', function($scope, $http, SERVER) {

  $http({
      url: SERVER.url + "api/news",
  })
  .success(function(data) {
      $scope.news = data.news;
  });

  $scope.test = function () {
    $http({
        url: SERVER.url + "api/is_logged",
    })
    .then(function(res) {
      console.log(res);
        $scope.vartest = res;
    });
  };

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
        LoginService.loginUser($scope.data.username, $scope.data.password).success(function(data) {
            $state.go('tab.news');
        }).error(function(data) {
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
        console.log(requestData);
        RegisterService.registerUser(requestData).success(function(data) {
            var alertPopup = $ionicPopup.alert({
              title: 'Register failed!',
              template: 'Please check your information!'
            });
            alertPopup.then(function(res) {
              $state.go('login');
            });
        }).error(function(data) {
            var alertPopup = $ionicPopup.alert({
              title: 'Register failed!',
              template: 'Please check your information!'
            });
        });
    };
});

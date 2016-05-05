angular.module('hermod.controllers', [])

.controller('GamesCtrl', function($scope, Games, $http) {
  // With the new view caching in Ionic, Controllers are only called
  // when they are recreated or on app start, instead of every page change.
  // To listen for when this page is active (for example, to refresh data),
  // listen for the $ionicView.enter event:
  //
  //$scope.$on('$ionicView.enter', function(e) {
  //});
  $http.get('js/challenges.json')
        .then(function(res){
          $scope.games = res.data;              
      });

  //$scope.games = Games.all();
  $scope.remove = function(game) {
    Games.remove(game);
  };
})

.controller('GameDetailCtrl', function($scope, $stateParams, GameService) {
  $scope.rank = GameService.getRankMock($stateParams.gameName);
  $scope.gameName = $stateParams.gameName;
})

.controller('LoginCtrl', function($scope, LoginService, $ionicPopup, $state) {
    $scope.data = {};
 
    $scope.login = function() {
        LoginService.loginUser($scope.data.username, $scope.data.password).success(function(data) {
            $state.go('games');
        }).error(function(data) {
            var alertPopup = $ionicPopup.alert({
                title: 'Login failed!',
                template: 'Please check your credentials!'
            });
        });
    };
});

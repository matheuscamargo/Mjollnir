angular.module('hermod.controllers', [])

.controller('NewsCtrl', function($scope, $http, API, NewsMock) {

  $http({
      url: API.url + "/api/news",
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

.controller('ChallengeDetailCtrl', function($scope, API, $stateParams, $http, ChallengesMock) {

  $scope.challengeName = $stateParams.challengeName;

  $http({
      url: API.url + "/api/challenge/" + $stateParams.challengeName,
  })
  .success(function(data) {
      $scope.rank = data.rank;
  });

  // Mock
  // $scope.rank = ChallengesMock.getRank();
})

.controller('GroupsCtrl', function($scope, $http, API, $state) {

  $http.get(API.url + "/api/groups", {})
  .success(function(data){
    $scope.groups = data.groups;   
  });

  $scope.join = function(groupId) {
    $http.get(API.url + "/api/join/" + groupId, {  })
    .success(function(data){
      for (var i = 0; i < $scope.groups.length; i++) {
        if ($scope.groups[i].id == groupId)
           $scope.groups[i].situation = ($scope.groups[i].situation == 'Join' ? 'Leave' : 'Join');
         }   
    });
  };

  $scope.enter = function(groupId) {
     $state.go('group.description', {id: groupId});
  };
})

.controller('GroupDescriptionCtrl', function ($scope, $rootScope, $stateParams, $http, API, $state) {
  //console.log($state.current.name)
  $http.get(API.url + "/api/group/" + $stateParams.id, { })
  .success(function(data){
    // $rootScope.group = data;
    $scope.group = data;
  });
})

.controller('GroupPlayCtrl', function ($scope, $stateParams, $rootScope, $http, API, $state) {
  //console.log($state.current.name)
  if ($state.current.name != 'group.play') {
     $state.go('group.play', {id: $stateParams.id});
  }
  $http.get(API.url + "/api/group/" + $stateParams.id, { })
  .success(function(data){
    $scope.group = data;
  });

})

.controller('GroupTournamentsCtrl', function ($scope, $stateParams, $rootScope, $http, API) {

  $http.get(API.url + "/api/group/" + $stateParams.id, {})
  .success(function(data){
    $scope.group = data;
  });

})

.controller('TournamentDetailCtrl', function($scope, API, $stateParams, $http, ChallengesMock) {

  $scope.tournamentId = $stateParams.tournamentId;

  $http({
      url: API.url + "/api/tournament/" + $stateParams.tournamentId,
  })
  .success(function(data) {
      for (i = 0; i < data.ranking.length; i++) { 
          data.ranking[i][0].rank = i + 1;
      }
    //console.log(data);
      $scope.ranking = data.ranking;
      $scope.matches = data.matches;
  });

  // Mock
  // $scope.rank = ChallengesMock.getRank();
})


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

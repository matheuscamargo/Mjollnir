controllerModule

.controller('GroupPlayCtrl', function ($scope, $stateParams, $rootScope, $http, API, $state, $ionicPopup) {
  //console.log($state.current.name)
  $scope.data = {}; 

  if ($state.current.name != 'group.play') {
     $state.go('group.play', {id: $stateParams.id});
  }
  $http.get(API.url + "/api/group/" + $stateParams.id, { })
  .success(function(data){
    $scope.group = data;
  });

  $scope.play = function() {
    $http.post(API.url + "/api/group/" + $stateParams.id, {
      challenge: $scope.data.challenge,
      rounds: $scope.data.rounds,
      opponent: $scope.data.opponent,
      cid: $scope.data.challenge
    })
    .success(function(data){
      console.log(data);
      if (data.error) {
        var errorPopup = $ionicPopup.alert({
          title: 'Erro',
          template: data.error
        });
      }
      else {
        var successPopup = $ionicPopup.alert({
          title: 'Jogo Enviado'
        });
      }
    });


  };

});
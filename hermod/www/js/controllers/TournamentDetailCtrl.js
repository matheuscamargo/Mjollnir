controllerModule

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
});
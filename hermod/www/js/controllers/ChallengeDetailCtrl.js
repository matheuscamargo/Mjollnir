controllerModule

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
});
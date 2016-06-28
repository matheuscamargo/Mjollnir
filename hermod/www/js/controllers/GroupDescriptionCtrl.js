controllerModule

.controller('GroupDescriptionCtrl', function ($scope, $rootScope, $stateParams, $http, API, $state) {
  //console.log($state.current.name)
  $http.get(API.url + "/api/group/" + $stateParams.id, { })
  .success(function(data){
    // $rootScope.group = data;
    $scope.group = data;
  });
});
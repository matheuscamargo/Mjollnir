controllerModule

.controller('NewsCtrl', function($scope, $http, API, NewsMock) {

  $http({
      url: API.url + "/api/news",
  })
  .success(function(data) {
      $scope.news = data.news;
  });

  // Mock
  // $scope.news = NewsMock.getNews();

});
'use strict';
var app = angular.module('BuzzFeederApp', ['angularMoment', 'ui.bootstrap']);
app.controller('BuzzCtrl', ['$scope', function($scope) {
    Parse.initialize("nRvq1YJfh1PhNNpeKoEIKfQDNxNsticnB01rddNI", "JxA7Xrv0Nsp4pa5ErxJBjygXQmwlLF5eiFQPaCcW");
    Parse.$ = jQuery;
    $scope.favourites = [];
    $scope.readLater = [];
    if (localStorage.favourites) {
        $scope.favourites = JSON.parse(localStorage.getItem("favourites"));
    }
    if (localStorage.readLater) {
        $scope.readLater = JSON.parse(localStorage.getItem("readLater"));
    }
    console.log($scope.favourites);


    $scope.initIFrame = function() {
        chrome.tabs.query({
            active: true,
            currentWindow: true
        }, function(tabs) {
            var expiry = new Date(parseInt(localStorage.expiryTime));
            var now = new Date();
            if (localStorage.accessToken && now < expiry) {
                if (!localStorage.userId) {
                    $.get("https://graph.facebook.com/v2.3/me?fields=picture,id,name", {
                            access_token: localStorage.accessToken
                        })
                        .done(function(data) {
                            var buzzObject = Parse.Object.extend("Buzzeszz");
                            var buzz = new buzzObject();
                            buzz.save({
                                FBUserId: data.id,
                                FBName: data.name,
                                FBProfilePic: data.picture.data.url
                            }).then(function(object) {
                                console.log('user saved!');
                                localStorage.userId = data.id;
                                localStorage.userName = data.name;
                                localStorage.userPicture = data.picture.data.url;
                            });

                        });
                } else

                {
                    console.log('hello!');
                    $.get("https://graph.facebook.com/v2.3/me/friends/", {
                            access_token: localStorage.accessToken
                        })
                        .done(function(resp) {
                            var ids = [];
                            console.log(resp);
                            console.log(resp.data);

                            for (let i = resp.data.length - 1; i >= 0; i--) {
                                console.log(i);
                                ids.push(resp.data[i].id);
                            }
                            console.log(ids);

                            var query = new Parse.Query("Buzzeszz");
                            query.containedIn("FBUserId", ids);
                            query.descending("createdAt");
                            query.find({
                                success: function(results) {
                                    console.log(results);
                                    $scope.$apply(function() {
                                        $scope.posts = results;
                                    });
                                },
                                error: function(err) {
                                    console.log(err);
                                }
                            });

                        });
                }
            } else {
                // $('#frame').hide();
                // $('#theactualthing').show();
                loginfacebook($scope.initIFrame);
            }
        });
    }
    $scope.initIFrame();
    $scope.saveToBeReadLater = function(post) {
        var UserObject = Parse.Object.extend("Buzzeszz"); //Change to User
        var query = new Parse.Query(UserObject);
        query.equalTo("FBUserId", localStorage.userId);
        console.log(localStorage.userId);
        query.find({
            success: function(results) {
                let userObjectId = results[0].id;
                var buzzObject = Parse.Object.extend("ReadLater");
                var buzz = new buzzObject();
                buzz.save({
                    UserID: userObjectId,
                    BuzzID: post.id,
                });
                let present = false;
                let readLater = $scope.readLater;
                for (let i = readLater.length - 1; i >= 0; i--) {
                  if(readLater[i].objectId === post.id){
                    present = true;
                  }  
                }
                if(!present){
                  $scope.readLater.push(post);
                  localStorage.removeItem(readLater);
                  localStorage.setItem("readLater", JSON.stringify(readLater));
                  $scope.readLater = JSON.parse(localStorage.getItem("readLater"));
                }
                
            },
            error: function(error) {
                console.log("Error: " + error.code + " " + error.message);
            }
        });
    };

    $scope.addToFavourites = function(post) {
        var UserObject = Parse.Object.extend("Buzzeszz"); //Change to User
        var query = new Parse.Query(UserObject);
        query.equalTo("FBUserId", localStorage.userId);
        console.log(localStorage.userId);
        query.find({
            success: function(results) {
                let userObjectId = results[0].id;
                var buzzObject = Parse.Object.extend("Favourites");
                var buzz = new buzzObject();
                buzz.save({
                    UserID: userObjectId,
                    BuzzID: post.id,
                });
                let present = false;
                let favourites = $scope.favourites;
                for (let i = favourites.length - 1; i >= 0; i--) {
                  if(favourites[i].objectId === post.id){
                    present = true;
                  } 
                }
                if(!present){
                  $scope.favourites.push(post);
                  localStorage.removeItem(favourites);
                  localStorage.setItem("favourites", JSON.stringify(favourites));
                  $scope.favourites = JSON.parse(localStorage.getItem("favourites"));
                }
            },
            error: function(error) {
                console.log("Error: " + error.code + " " + error.message);
            }
        });
    };
}]);


app.filter('fromNow', function() {
    return function(date) {
        return moment(date).fromNow();
    }
});
// document.addEventListener('DOMContentLoaded', function() {

// });
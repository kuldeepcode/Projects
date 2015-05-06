'use strict';

chrome.runtime.onInstalled.addListener(function(details) {
    console.log('previousVersion', details.previousVersion);
});

// chrome.browserAction.setBadgeText({
//     text: '1'
// });

chrome.tabs.onUpdated.addListener(function(tabId, changeInfo, tab) {
    if (changeInfo.status === 'complete') {
        if (tab.url.toLowerCase().indexOf('buzzfeed.com') > -1) {
            var thatURL = tab.url;

            if (thatURL === 'http://www.buzzfeed.com/' || thatURL === 'http://www.buzzfeed.com/lol' || thatURL === 'http://www.buzzfeed.com/news' || thatURL === 'http://www.buzzfeed.com/buzz' || thatURL === 'http://www.buzzfeed.com/life' || thatURL === 'http://www.buzzfeed.com/animals' || thatURL === 'http://www.buzzfeed.com/win' || thatURL === 'http://www.buzzfeed.com/omg' || thatURL === 'http://www.buzzfeed.com/cute' || thatURL === 'http://www.buzzfeed.com/fail' || thatURL === 'http://www.buzzfeed.com/wtf' || thatURL === 'http://www.buzzfeed.com/trending') {

                return;
            }

            thatURL = thatURL.split('#')[0];


            $.get("http://www.buzzfeed.com/buzzfeed/api/buzz", {
                    session_key: '6d7e4350b90517f0061749ecdf0145efdc0d7758b9f1f6226f5a5b1bad6267b9hackathon4',
                    url: thatURL
                })
                .done(function(resp) {

                    var buzzurll = 'http://buzzfeed.com/' + resp.buzz.username + '/' + resp.buzz.uri;
                    var buzzdescpt = resp.buzz.description.replace('<b>','').replace('</b>','');               
                    Parse.$ = jQuery;

                    Parse.initialize("nRvq1YJfh1PhNNpeKoEIKfQDNxNsticnB01rddNI", "JxA7Xrv0Nsp4pa5ErxJBjygXQmwlLF5eiFQPaCcW");

                    var FBId = localStorage.userId;
                    var FBName = localStorage.userName;
                    var FBPic = localStorage.userPicture;

                    var buzzObject = Parse.Object.extend("Buzzeszz");
                    var buzz = new buzzObject();
                    buzz.save({
                        FBUserId: FBId,
                        FBName: FBName,
                        FBProfilePic: FBPic,
                        Buzz: buzzurll,
                        BuzzImage: resp.buzz.image,
                        BuzzDescription: buzzdescpt,
                        BuzzTitle: resp.buzz.title
                    }).then(function(object) {
                        console.log(object);
                        console.log('buzz saved!');
                    });
                });
            console.log('you got logged');


        }
    }
});
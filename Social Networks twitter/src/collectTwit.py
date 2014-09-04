# Last edit by Marcos Kohler 4_13_2014

import tweepy
import distance
import sys
import csv
import math
import getopt

consumer_key = "IvnfDQOf0aVOPJ5Vz7buaQ"
consumer_secret = "2FCLK3EXQNjHyE1qP1rMglOLHOSsRGnC7sNYNZiuI"

access_token_key = "15400274-5IgFzMdT24bzwZLPUsthrtz8GbtucRFy1IGfpnsJe"
access_token_secret = "YzV5zZWuhoojiBkdi5yb5ZnChKOoyshpvvT2ZRphTKUFi"

auth1 = tweepy.OAuthHandler(consumer_key,consumer_secret)
auth1.set_access_token(access_token_key,access_token_secret)

# need this line to access user and followers
api = tweepy.API(auth1)

center = {'lat' : 40.7300,'long' : -73.9950} 		# Washington Square Park
setTerms = ['sale']                                 # Keywords
setLanguages = ['en']								# Language Filter

# flags
verbose = False
collectTrainingData = False

# global variables
tweetThresh = 100                                   # Number of tweets to collect

class StreamListener(tweepy.StreamListener):

    def __init__(self):
        super(StreamListener, self).__init__()		# Not sure if needed
        self.tweetCount = 0							# Tweets tracked
        self.tweetRadius = 1000                     # Tweet tracking limit

    def on_status(self,tweet):
        self.text = ""
        self.hashtags = ""
        self.favs = 0
        self.retweets = 0
        self.username = ""
        self.followers = 0

        if (self.tweetCount < tweetThresh):

            if tweet.coordinates:

                d = math.ceil(distance.distance_between(tweet.coordinates["coordinates"][1], tweet.coordinates["coordinates"][0], center["lat"], center["long"]))

                if d < self.tweetRadius:
                    
                    # General Information
                    self.tweetCount += 1
                    self.text = tweet.text
                    self.favs = tweet.favorite_count
                    self.retweets = tweet.retweet_count
                    
                    # User Data
                    user = api.get_user(tweet.user.screen_name)
                    if user:
                        self.username = user.screen_name
                        self.followers = user.followers_count
                    
                    if verbose:
                        print "Tweet: " + str(self.tweetCount)
                        print "Text: " + self.text + " (Distance: " + str(d) + " mi)"
                        print "Fav: " + str(self.favs)
                        print "Retweets: " + str(self.retweets)
                        print "Hashtags: " + self.hashtags
                        print "User: " + self.username
                        print "Followers: " + str(self.followers)
                        print "\n"
                
                
                    # CSV output
                    label = 1  # used for trainning classfilier
                    csvFile = 'testData.csv'
                    
                    if collectTrainingData:
                        csvFile = 'trainData.csv'
                    if (self.tweetCount == 0):
                        with open(csvFile,'wb') as outfile:
                            writer = csv.writer(outfile,delimiter='|')
                            writer.writerow((label,self.text.encode('utf-8'),self.followers))
                    else:
                        fd = open(csvFile,'a')
                        writer = csv.writer(fd,delimiter='|')
                        writer.writerow((label,self.text.encode('utf-8'),self.followers))
                        fd.close()

        else:
            print("Done Collecting Tweets!\n")
            sys.exit()
    #end on_status

    def on_error(self, status_code):
        print "Error: " + repr(status_code)
        return False
    #end on_error

def main(argv):
    
    global verbose
    global collectTrainingData
    global tweetThresh
	
    # Process command-line arguments
    try:
        opts,args = getopt.getopt(argv,"vt")
    except getopt.GetoptError:
        print 'Error'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-v':
            verbose = True
        elif opt == '-t':
            collectTrainingData = True
    
	l = StreamListener()
	streamer = tweepy.Stream(auth=auth1,listener=l)
	streamer.filter(track=setTerms,languages=setLanguages)

# end main

if __name__ == "__main__":
    main(sys.argv[1:])
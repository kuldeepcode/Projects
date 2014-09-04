from nltk.tokenize import wordpunct_tokenize
import re
import nltk

class Record:

    def __init__(self,label,message,followers):
        
        self.label = label
        self.message = message
        self.followers = followers
        self.features_set = {}              # Dictionary used by classifier
        self.hashtags = []                  # Words marked by hashtags
        self.probability = 0.0              # Relevance to the label '1'
        self.impact_score = 0.0             # Probability * (followers + 1)
        
        # Get set of unique words contained in a tweet
        self.message_words = set(wordpunct_tokenize(self.message.replace('\r',' ').lower()))
        
        # Get rid of punctuation,tokens,numbers, and single letters.
        self.message_words = [w for w in self.message_words if re.search('[a-zA-Z]', w) and len(w) > 1]

    # Generate a tweet's features_set according to input features
    def get_features_set(self,features_set):
        for k,v in features_set.items():
            if k in self.message_words:
                self.features_set[k] = 1
            else:
                self.features_set[k] = 0

class Ranker:

    def __init__(self,training_data_csv,test_data_csv,min_len=1,min_frequency=3,topNum=5):
        
        self.training_data_csv = training_data_csv
        self.test_data_csv = test_data_csv
        self.features_set = {}                              # Generated from training data, unique words to indicate a specific event
        self.training_data = []                             # Generated from training_data_csv file, each twitter message is a training record
        self.classified_records_list = []                   # Classified twitter records
        self.topN_records = []                              # Records having the top N impact score
        self.topNum = topNum                                # Length of topN_records list
        self.classifier = None
        self.nonsense_words = ['and','the','http','co']     # Nonsense words needed to be eliminated from features_set
        self.min_len = min_len                              # Length of words in features_set are greater than min_len
        self.min_frequency = min_frequency                  # Frequency of words in features_set are greater than min_frequency

    # From input training file, create words in features_set and training_data list
    def __construct_training_data(self):
        records_list = []
        word_counts= {}
        # Read csv file, create records list
        with open(self.training_data_csv,'rb') as csvfile:
            for line in csvfile.readlines():
                fields = line.split('|')
                if len(fields) == 3:
                    record = Record(fields[0],fields[1],fields[2])
                    records_list.append(record)

        # Get word count in all messages
        for record in records_list:
            for word in record.message_words:
                if word in word_counts:
                    word_counts[word] += 1
                else:
                    word_counts[word] = 1

        # Eliminate words whose count is less than min_frequency
        for k, v in word_counts.items():
            if v > self.min_frequency and k not in self.nonsense_words:
                self.features_set[k] = v

        # Create training data for NLP model
        for record in records_list:
            # Construct feature set for evey record
            record.get_features_set(self.features_set)
            self.training_data.append((record.features_set,record.label))

    # Ese maximum entropy classify a twitter to different events set
    # Here we only have two events : 1 or 0
    # Event 1 means the tweet is relevant
    # Event 0 means the tweet is not relevant
    # Probability of event 1 + probability of event 0 = 1
    def __get_classify_probability(self,records_list,algorithm='GIS'):
        try:
            if self.classifier == None:
                self.classifier = nltk.classify.MaxentClassifier.train(self.training_data,algorithm,trace = 0,max_iter=200)
        except Exception as e:
            print 'Error: %r' %e
            return

        for record in records_list:
            probability = self.classifier.prob_classify(record.features_set)
            record.probability = probability.prob('1')
            # print record.message
            # print '%6.2f, %6.2f' %(probability.prob('1'),probability.prob('0'))
        self.classified_records_list = records_list

    # Calculate impact scores of tweets in test data
    # Generate classified_records_list and topN_records
    def score_test_data(self):
        self.__construct_training_data()

        # Read csv file, create records list
        records_list = []
        with open(self.test_data_csv,'rb') as csvfile:
            for line in csvfile.readlines():
                fields = line.split('|')
                if len(fields) == 3:
                    record = Record(fields[0],fields[1],fields[2])
                    record.get_features_set(self.features_set)
                    records_list.append(record)

        # Classify tweets and get their probability distance for different labels
        self.__get_classify_probability(records_list)
        records_list = sorted(records_list[:],key=lambda record : record.probability,reverse=True)
        for r in records_list:
            try:
                r.impact_score = r.probability * (float(r.followers)+1)
            except Exception as e:
                r.impact_score = r.probability
        self.classified_records_list = records_list

        # Get topN impact score record
        topN = []
        for i in range(0,self.topNum):
            if i < len(records_list):
                topN.append(records_list[i])
                # print topN[i].impact_score

        self.topN_records = topN

    # Demonstrate getting top 5 messages related to event 1
    def demo_print_topN_message(self):
        self.score_test_data()
        print 'Top ',topNum,' impact score twitter messages are: '
        for r in ranker.topN_records:
            print r.message
            print 'impact score: ', r.impact_score

    def classify_message_live_stream(self, message_row, algorithm = 'GIS'):
        # Contruct classifier
        if len(self.training_data)== 0 :
            self.__construct_training_data()
        try:
            if self.classifier == None:
                self.classifier = nltk.classify.MaxentClassifier.train(self.training_data,algorithm,trace = 0,max_iter=200)
        except Exception as e:
            print 'Error: %r' %e
            return
        # Get message's features set
        # Classify message
        fields = message_row.split('|')
        try:
            record = Record(fields[0],fields[1],fields[2])
            record.get_features_set(self.features_set)
            probability = self.classifier.prob_classify(record.features_set)
            record.probability = probability.prob('1')
            
            try:
                record.impact_score = record.probability * (float(record.followers)+1)
            except Exception as e:
                record.impact_score = record.probability
            
            if record.probability > 0.85:
                print record.message,'\n','impact score: ',record.impact_score

        except Exception as e:
            print 'Error: %r' %e   

    def classify_live_stream_demo(self):
        with open(self.test_data_csv,'rb') as csvfile:
            for line in csvfile.readlines():
                self.classify_message_live_stream(line)

if __name__=='__main__':
    
    training_data_csv = 'data/trainData_topic_party.csv'
    test_data_csv = 'data/testData_beer_party_fire.csv'
    min_len = 1
    min_frequency = 3
    topNum = 10
    ranker = Ranker(training_data_csv,test_data_csv,topNum=topNum)
    # ranker.demo_print_topN_message()
    ranker.classify_live_stream_demo()
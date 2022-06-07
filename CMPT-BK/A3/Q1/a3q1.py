

if __name__=='__main__':

    with open("four.txt","r") as f:
        #save legal words
        valid_words=[line.strip() for line in f.readlines()]

    filename=input("Name of chain file to check?")
    with open(filename,"r") as f:
        #save the words to be verified
        word_chain=[line.strip() for line in f.readlines()]
    if len(word_chain)<2:
        print("Invalid: too short")
        exit()
    #It is used to record the occurrence of word and compare the word with count[-1]
    count=[]
    for word in word_chain:
        if word not in valid_words:
            print(f"Invalid word: {word}")
            exit()
        if word in count:
            print(f"Invalid: repeated word:{word}")
            exit()
        else:
            #Compare from the second word
            if len(count)>0:
                diff=0
                comp_ee=list(count[-1])
                comp_er=list(word)
                #Assume that the length of words is equal
                for i in range(len(comp_ee)):
                    if comp_ee[i]!=comp_er[i]:
                        diff+=1
                # Adjacent strings can only differ by one character 
                if diff!=1:
                    print(f"Invalid step from {''.join(comp_ee)} -> {''.join(comp_er)}")
                    exit()
            count.append(word)
    print("Valid!")


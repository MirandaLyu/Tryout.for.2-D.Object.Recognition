# practice-for-Content-based-Image-Retrieval

This was a CV class project as well. But it took me a lot of time to build. Good amounts of time were spent on reading textbooks (trying to understand the mechanisms of thresholding, morphological operations, the least central moment, etc.) and reading OpenCV documentations.

## general design

The CBIR system was implemented as a command line program. It runs with 5 inputs which are
* *a target image*
* *the image database*
* *a feature calculation method*
* *a distance metric*
* *how many top N matching images you want*

The target image is the one you want to find similar photos for, the feature method is a way to get a photo's mathematical feature, and the distance metric is a way to compare two photos' features. The image database in this project is [here](https://drive.google.com/drive/folders/1rsZKCSA6oTLglI1IN_BhKH9EPqGt27dU?usp=sharing).

## first, try a simple matching method

This baseline matching only extracts the middle 9x9 pixels in every picture as the feature, and uses sum-of-squared-difference to compare every two features (do calculations for every pixel and add the differences together). One example of the output is this:

<img width="220" alt="Screenshot 2024-03-10 at 9 13 25 PM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/ef759054-77c4-408b-a30e-e834ea062f86">

The upper left corner is the target image and others are the top 3 matches. We can see that the middles are indeed all red. And this is the metric calculated:

<img width="350" alt="Screenshot 2024-03-11 at 1 42 35 AM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/c58abdd7-7898-443f-aec2-03bf7972c7bb">

## now, color matching

Color matching usually uses histogram so I built a 3-D 8 bins histogram for each image as feature vector. Then every two images are compared using a metric called histogram intersection (the larger the intersection, the more the similarity).

One example of the output is like this:

<img width="220" alt="Screenshot 2024-03-11 at 1 56 33 AM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/20f622ad-5645-4ba8-8775-62bc808ac151">

with the following stats:

<img width="350" alt="Screenshot 2024-03-12 at 12 17 56 AM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/5ef398d6-a3f0-4ee5-b7a5-344dd6e815cb">

We can see that these color distributions are very alike. But the lack of spatial precision is the disadvantage of color histograms.

## so let's try add a little spatial information

In order to improve the matching precision, this time I let the system to produce two color histograms for each picture. One represented the upper 1/2 part of the image, the other represented the bottom. They later were compared separately using histogram intersection and the final distance was the sum of two comparisons.

And one of the output looks like this:

<img width="250" alt="Screenshot 2024-03-12 at 12 27 35 AM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/780deb84-36e8-45df-964f-d190566c489a">

We can see that after the feature being separated to two regions, the similarity is a little more obvious, especially the top match 2 picture (top match 1 is the original target picture). This also reflects in the stats:

<img width="350" alt="Screenshot 2024-03-12 at 12 27 46 AM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/1ec9e608-55a9-4553-8bd3-c88848e7c245">

## compare texture as well

Color and spatial layout are two of the attributes of a picture. Texture is another one. But how to compare texture?

The approach used here is to calculate the Sobel magnitude image (Sobel detects edges) and use a histogram of gradient magnitudes as the texture feature. 16 bin size was used here.

So we can compare 3-D whole image color histograms and 1-D texture histograms together this time. The results were added then. This is one example of the output:

<img width="250" alt="Screenshot 2024-03-12 at 1 02 07 AM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/e0284848-1da7-4225-899b-27fffa800644">

This result may not be obvious. I also ran the same photo with previous color matching methods:

<img width="250" alt="Screenshot 2024-03-12 at 1 02 24 AM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/f1a67e7e-e8e8-4efd-a2a5-577994e56daf">

Now we can see that texture indeed adds a layer of complexity in matching. Though still losing spatial precision.

## final try

Finally, I decided to try gradient orientations as well. I designed the texture histogram to be a 2-D histogram with both magnitudes and orientations information (8 bins each dimension). And in order to compare texture more precisely, I divided the whole image into 4 equal regions and concatenated 4
regions’ texture histograms into a final one. 

For color, I still use the whole image 3-D method. Then two results are these:

<img width="270" alt="Screenshot 2024-03-12 at 1 18 48 AM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/3242b829-7a84-4125-bde2-8f0b6d8cf51d">

<img width="270" alt="Screenshot 2024-03-12 at 1 19 00 AM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/5b2f9a70-b664-445d-bc72-3e042baf5eaa">

After long battling with math, I was satisfied with the results, especially the second one.

## FINAL THOUGHT

* Probably from the description, you can also see that matching method can be more and more precise. There must be a lot of people working on this.
* I invested a lot of time learning and doing this project 😵 , but I enjoyed it in the end because I felt more like a real science person. Since I knew very little about computer vision, I read paper and textbook first to understand concepts and methodologies. So the whole process was like using papers, ChatGPT, youTube and class recordings together. But it really improved research skills.

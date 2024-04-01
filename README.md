# tryout-for-2D-object-recognition

This was a CV class project as well. But it took me a lot of time to build.

Good amounts of time were spent on reading textbooks (trying to understand the mechanisms of thresholding, morphological operations, the least central moment, etc.) and reading OpenCV documentations (understanding how to correctly use some functions).

## general design

My object recognition system was designed as a command line program.

It takes in a folder of images and either saves the objects' features to databse OR gives unknown objects matching labels.

*All images are white background and contain only one object

## first, thresholding the image

There are actually a lot of ways to find a relatively optimal threshold for an image. I have tried many and I found Otsu’s Method work best with my training images.

So I preprocessed every image a little first (blurring a little and making strongly colored pixels be darker) and then applied Otsu’s Method to binary images from scratch (not using the built-in function).

Here are examples of the original vs thresholded images:

<img width="200" alt="Screenshot 2024-03-31 at 9 12 20 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/93cccc8a-70fe-4743-8a32-516fb2559402">
<img width="100" alt="Screenshot 2024-03-31 at 9 12 32 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/f80183e6-1120-4141-8fb4-ccab12be6aa6">

<img width="200" alt="Screenshot 2024-03-31 at 9 12 41 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/d3ef21c1-2e95-452f-b736-e8dbd96033ee">
<img width="100" alt="Screenshot 2024-03-31 at 9 12 50 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/67edfd39-db11-4009-b40b-280e86d5ac42">

## second, clean up

Cleaning up is a very skilled technique. It can use layers of different morphological operations to deal with complicated shapes.

But since here my thresholded images are already doing a good job, I just used the Closing operation to fill the small holes in images.

Results:

<img width="200" alt="Screenshot 2024-03-31 at 9 25 25 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/8e492630-2d85-4eb9-9926-353c89a4bb19">
<img width="102" alt="Screenshot 2024-03-31 at 9 25 33 PM" src="https://github.com/MirandaLyu/Tryout.for.2-D.Object.Recognition/assets/115821003/b4557546-9120-43af-b3aa-041a98c4cb18">

## third, segmentation

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

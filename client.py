#!/usr/bin/env python3


import time
from concurrent.futures import ThreadPoolExecutor, as_completed

import requests

concurrent_tasks = 2

url = "http://localhost:8080"


# Function to fetch a single URL and return the response and time taken
def fetch_url(url):
    start_time = time.time()  # Start time before making the request
    response = requests.get(url)
    end_time = time.time()  # End time after receiving the response
    elapsed_time = end_time - start_time  # Time taken for the request
    return response.status_code, elapsed_time


# Use a ThreadPoolExecutor to make requests concurrently
with ThreadPoolExecutor(max_workers=concurrent_tasks) as executor:
    # Submit the requests to the executor
    future_to_url = {
        executor.submit(fetch_url, url): id for id in range(concurrent_tasks)
    }

    # Track the total time for all requests
    total_start_time = time.time()

    # Collect the results as they complete
    for future in as_completed(future_to_url):
        id = future_to_url[future]
        try:
            # Get the result and the time taken from the future
            result, elapsed_time = future.result()
            print(
                f"fetched data from {id}: {result}, time taken: {elapsed_time:.2f} seconds"
            )
        except Exception as e:
            print(f"error fetching data from {id}: {e}")

    total_end_time = time.time()  # End time after all requests are processed
    total_elapsed_time = total_end_time - total_start_time  # Total time taken
    print(f"\ntotal time for all requests: {total_elapsed_time:.2f} seconds")

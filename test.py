#!/usr/bin/env python3

import statistics
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

import requests

concurrent_tasks = 200

url = "http://localhost:8080"


# Function to fetch a single URL and return the response and time taken
def fetch_url(url):
    status_code = -1
    start_time = time.time()  # Start time before making the request
    try:
        status_code = requests.get(url).status_code
    except:
        ...
    end_time = time.time()  # End time after receiving the response
    elapsed_time = end_time - start_time  # Time taken for the request
    return status_code, elapsed_time


elapsed_times = []
result_codes = []

# Use a ThreadPoolExecutor to make requests concurrently
with ThreadPoolExecutor() as executor:
    print(
        f"concurrent requests: {concurrent_tasks} max_workers: {executor._max_workers}"
    )
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
            status_code, elapsed_time = future.result()
            elapsed_times.append(elapsed_time)  # Collect the time for this worker
            result_codes.append(status_code)
            # print(
            #     f"fetched data from {id}: {result}, time taken: {elapsed_time:.2f} seconds"
            # )
        except Exception as e:
            print(f"error fetching data from {id}: {e}")

    total_end_time = time.time()  # End time after all requests are processed
    total_elapsed_time = total_end_time - total_start_time  # Total time taken

    print(f"total time for all requests: {total_elapsed_time:.2f} seconds")
    # Calculate succes and failur
    print(f"response fail: {result_codes.count(-1)}")
    print(f"response success: {result_codes.count(200)}")
    # Calculate the average time for all workers
    print(f"resposne mean: {statistics.mean(elapsed_times):.2f} seconds")
    print(f"response median: {statistics.median(elapsed_times):.2f} seconds")
